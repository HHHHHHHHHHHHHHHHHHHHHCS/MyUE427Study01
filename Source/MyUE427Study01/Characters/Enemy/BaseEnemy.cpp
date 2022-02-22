// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Engine/SkeletalMeshSocket.h"

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

	leftAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftAttackCollsion"));
	leftAttackCollision->SetupAttachment(GetMesh(), FName("LeftAttackSocket"));
	DeactiveLeftAttackCollision();

	rightAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightAttackCollsion"));
	rightAttackCollision->SetupAttachment(GetMesh(), FName("RightAttackSocket"));
	DeactiveRightAttackCollision();

	healthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	healthBarWidgetComponent->SetupAttachment(GetRootComponent());
	healthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	healthBarWidgetComponent->SetDrawSize(FVector2D(125.0f, 10.0f));

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

	bAttackVolumeOverlapping = false;
	interpSpeed = 15.0f;
	bInterpToPlayer = false;

	maxHealth = 150.0f;
	health = maxHealth;

	damage = 10.0f;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	ChaseVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapBegin);
	ChaseVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapEnd);

	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapBegin);
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapEnd);

	leftAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnLeftAttackCollisionOverlapBegin);
	leftAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnLeftAttackCollisionOverlapEnd);

	rightAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnRightAttackCollisionOverlapBegin);
	rightAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnRightAttackCollisionOverlapEnd);

	healthBar = Cast<UProgressBar>(healthBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName("HealthBar"));
	healthBar->SetPercent(health / maxHealth);
	healthBar->SetVisibility(ESlateVisibility::Visible);

	AIController = Cast<AAIController>(GetController());
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterpToPlayer)
	{
		const FVector playerLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
		const float yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerLocation).Yaw;
		FRotator rot = GetActorRotation();
		rot.Yaw = FMath::FInterpTo(rot.Yaw, yaw, DeltaTime, interpSpeed);
		SetActorRotation(rot);
	}
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
			healthBar->SetVisibility(ESlateVisibility::Hidden);
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
			if (EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
			{
				MoveToTarget(mainPlayer);
			}
		}
	}
}

void ABaseEnemy::OnLeftAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if(damagedSet.Contains(mainPlayer))
			{
				return;
			}
			
			damagedSet.Add(mainPlayer);

			
			if (mainPlayer->hitParticles)
			{
				USkeletalMeshComponent* mesh = GetMesh();
				const USkeletalMeshSocket* attackSocket = mesh->GetSocketByName("LeftAttackSocket");
				if (attackSocket)
				{
					const FVector socketLocation = attackSocket->GetSocketLocation(mesh);
					UGameplayStatics::SpawnEmitterAtLocation(this, mainPlayer->hitParticles, socketLocation,
					                                         FRotator(0.0f));
				}
			}
			if (mainPlayer->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, mainPlayer->hitSound);
			}
			if (damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(mainPlayer, damage, AIController, this, damageTypeClass);
			}
		}
	}
}

void ABaseEnemy::OnLeftAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseEnemy::OnRightAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if(damagedSet.Contains(mainPlayer))
			{
				return;
			}
			
			damagedSet.Add(mainPlayer);
			
			if (mainPlayer->hitParticles)
			{
				USkeletalMeshComponent* mesh = GetMesh();
				const USkeletalMeshSocket* attackSocket = mesh->GetSocketByName("RightAttackSocket");
				if (attackSocket)
				{
					const FVector socketLocation = attackSocket->GetSocketLocation(mesh);
					UGameplayStatics::SpawnEmitterAtLocation(this, mainPlayer->hitParticles, socketLocation,
					                                         FRotator(0.0f));
				}
			}
			if (mainPlayer->hitSound)
			{
				UGameplayStatics::PlaySound2D(this, mainPlayer->hitSound);
			}
			if (damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(mainPlayer, damage, AIController, this, damageTypeClass);
			}
		}
	}
}

void ABaseEnemy::OnRightAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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

		// auto pathPoints = navPath->GetPathPoints();
		// for (auto point : pathPoints)
		// {
		// 	FVector location = point.Location;
		// 	UKismetSystemLibrary::DrawDebugSphere(this, location, 25.0f, 8, FLinearColor::Red, 10.0f, 1.5f);
		// }
	}
}

void ABaseEnemy::Attack()
{
	if (AIController)
	{
		AIController->StopMovement();
	}

	if (EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_Attacking;

		bInterpToPlayer = true;

		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && attackMontage)
		{
			damagedSet.Empty();

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
	bInterpToPlayer = false;
	if (bAttackVolumeOverlapping)
	{
		Attack();
	}
}

void ABaseEnemy::ActiveLeftAttackCollision()
{
	leftAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	leftAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	leftAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	leftAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ABaseEnemy::DeactiveLeftAttackCollision()
{
	leftAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::ActiveRightAttackCollision()
{
	rightAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	rightAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	rightAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	rightAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ABaseEnemy::DeactiveRightAttackCollision()
{
	rightAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	health = FMath::Clamp(health - DamageAmount, 0.0f, maxHealth);
	if(health<=0)
	{
		Die();
	}

	healthBar->SetPercent(health / maxHealth);

	return health;
}

void ABaseEnemy::Die()
{
}
