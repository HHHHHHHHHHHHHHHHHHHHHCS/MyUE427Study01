// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyUE427Study01/Characters/Player/MainPlayer.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChaseVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseVolume"));
	ChaseVolume->SetupAttachment(GetRootComponent());
	ChaseVolume->InitSphereRadius(800.0f);
	ChaseVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ChaseVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ChaseVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->InitSphereRadius(100.0f);
	AttackVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	ChaseVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapBegin);
	ChaseVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapEnd);

	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapBegin);
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapEnd);

	AIController = Cast<AAIController>(GetController());
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			// UE_LOG(LogTemp, Warning, TEXT("OnChaseVolumeOverlapBegin"));
			mainPlayer->UpdateAttackTarget();
			MoveToTarget(mainPlayer);
		}
	}
}

void ABaseEnemy::OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			// UE_LOG(LogTemp, Warning, TEXT("OnChaseVolumeOverlapEnd"));

			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// UE_LOG(LogTemp, Warning, TEXT("OnAttackVolumeOverlapBegin"));

		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			bAttackVolumeOverlapping = true;
			Attack();
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		// UE_LOG(LogTemp, Warning, TEXT("OnAttackVolumeOverlapEnd"));
	
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			bAttackVolumeOverlapping = false;
			if(EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
			{
				MoveToTarget(mainPlayer);
			}
		}
	}
}

void ABaseEnemy::MoveToTarget(AMainPlayer* targetPlayer)
{
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_MoveToTarget;
	if (AIController)
	{
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(targetPlayer);
		moveRequest.SetAcceptanceRadius(10.0f);
		FNavPathSharedPtr navPath;
		AIController->MoveTo(moveRequest, &navPath);

		auto pathPoints = navPath->GetPathPoints();
		for (auto point : pathPoints)
		{
			FVector location = point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, location, 25.0f, 8, FLinearColor::Red, 10.0f, 1.5f);
		}
	}
}

void ABaseEnemy::Attack()
{
	if(AIController)
	{
		AIController->StopMovement();
	}
	
	if (EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_Attacking;

		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && attackMontage)
		{
			float playRate = FMath::RandRange(0.9f, 1.1f);
			FString sectionName = FString::FromInt(FMath::RandRange(1, 3));
			animInstance->Montage_Play(attackMontage, playRate);
			animInstance->Montage_JumpToSection(FName(*sectionName), attackMontage);
		}
	}
}

void ABaseEnemy::AttackEnd()
{
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
	if(bAttackVolumeOverlapping)
	{
		Attack();
	}
}
