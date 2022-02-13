// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "MyUE427Study01/Gameplay/WeaponItem.h"
#include "MyUE427Study01/Characters/Enemy/BaseEnemy.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 600.0f;
	springArm->bUsePawnControlRotation = true;

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetCapsuleSize(35.0f, 100.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//转向加速度
	UCharacterMovementComponent* charaMovement = GetCharacterMovement();
	charaMovement->bOrientRotationToMovement = true;
	charaMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	charaMovement->JumpZVelocity = 500.0f;
	charaMovement->AirControl = 0.15f;

	baseTurnRate = 65.0f;
	baseLookUpRate = 65.0f;

	maxHealth = 100.0f;
	health = maxHealth;
	maxStamina = 150.0f;
	stamina = maxStamina;
	staminaConsumeRate = 20.0f;
	exhaustedStaminaRatio = 0.167f;
	staminaStatus = EPlayerStaminaStatus::EPSS_Normal;
	coins = 0;

	runningSpeed = 600.0f;
	sprintingSpeed = 900.0f;
	SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);

	bLeftShiftKeyDown = false;

	bHasWeapon = false;
	bIsAttacking = false;

	attackTarget = nullptr;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (staminaStatus)
	{
	case EPlayerStaminaStatus::EPSS_Normal:
		{
			if (bLeftShiftKeyDown)
			{
				if (stamina - staminaConsumeRate * DeltaTime < maxStamina * exhaustedStaminaRatio)
				{
					staminaStatus = EPlayerStaminaStatus::EPSS_Exhausted;
				}
				stamina = FMath::Max(stamina - staminaConsumeRate * DeltaTime, 0.0f);
				SetMovementStatus(EPlayerMovementStatus::EPMS_Sprinting);
			}
			else
			{
				stamina = FMath::Clamp(stamina + staminaConsumeRate * DeltaTime, 0.0f, maxStamina);
				staminaStatus = EPlayerStaminaStatus::EPSS_Normal;
				SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
			}
			break;
		}
	case EPlayerStaminaStatus::EPSS_Exhausted:
		{
			if (bLeftShiftKeyDown)
			{
				stamina = FMath::Max(stamina - staminaConsumeRate * DeltaTime, 0.0f);
				if (stamina <= 0.0f)
				{
					staminaStatus = EPlayerStaminaStatus::EPSS_ExhaustedRecovering;
					LeftShiftKeyUp();
					SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
				}
			}
			else
			{
				stamina = FMath::Clamp(stamina + staminaConsumeRate * DeltaTime, 0.0f, maxStamina);
				staminaStatus = EPlayerStaminaStatus::EPSS_ExhaustedRecovering;
				SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
			}
			break;
		}
	case EPlayerStaminaStatus::EPSS_ExhaustedRecovering:
		{
			stamina = FMath::Clamp(stamina + staminaConsumeRate * DeltaTime, 0.0f, maxStamina);
			if (stamina >= maxStamina * exhaustedStaminaRatio)
			{
				staminaStatus = EPlayerStaminaStatus::EPSS_Normal;
			}
			LeftShiftKeyUp();
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
	default: ;
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainPlayer::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::InteractKeyDown);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainPlayer::AttackKeyDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainPlayer::AttackKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	PlayerInputComponent->BindAxis("TurnAtRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AMainPlayer::LookUpAtRate);
}

void AMainPlayer::MoveForward(float value)
{
	if (bIsAttacking)
	{
		return;
	}

	if (Controller == nullptr || value == 0)
	{
		return;
	}

	// AddMovementInput(GetActorForwardVector(), value);
	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
	const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, value);
}

void AMainPlayer::MoveRight(float value)
{
	if (Controller == nullptr || value == 0)
	{
		return;
	}

	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
	const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, value);
}

void AMainPlayer::Turn(float value)
{
	if (value != 0)
	{
		AddControllerYawInput(value);
	}
}

void AMainPlayer::LookUp(float value)
{
	// UE_LOG(LogTemp, Warning, TEXT("%f,,,,%f"), GetControlRotation().Pitch, value);

	float pitch = GetControlRotation().Pitch;

	if (pitch > 45 && pitch < 180 && value < 0)
	{
		if (pitch > 45)
		{
			FRotator rotator = GetControlRotation();
			rotator.Pitch = 45;
			Controller->SetControlRotation(rotator);
		}
		return;
	}

	if (pitch < 270 && pitch >= 180 && value > 0)
	{
		if (pitch < 270)
		{
			FRotator rotator = GetControlRotation();
			rotator.Pitch = 270;
			Controller->SetControlRotation(rotator);
		}
		return;
	}

	if (value != 0)
	{
		AddControllerPitchInput(value);
	}
}

void AMainPlayer::TurnAtRate(float rate)
{
	const float value = rate * baseTurnRate * GetWorld()->GetDeltaSeconds();
	if (value != 0.0)
	{
		AddControllerYawInput(value);
	}
}

void AMainPlayer::LookUpAtRate(float rate)
{
	const float value = rate * baseLookUpRate * GetWorld()->GetDeltaSeconds();
	if (value != 0.0)
	{
		AddControllerPitchInput(value);
	}
}

void AMainPlayer::Jump()
{
	if (bIsAttacking)
	{
		return;
	}

	Super::Jump();
}

void AMainPlayer::IncreaseHealth(float value)
{
	health = FMath::Clamp(health + value, 0.0f, maxHealth);
}

void AMainPlayer::IncreaseStamina(float value)
{
	stamina = FMath::Clamp(stamina + value, 0.0f, maxStamina);
}

void AMainPlayer::IncreaseCoin(int value)
{
	coins += value;
}

float AMainPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                              AActor* DamageCauser)
{
	if (health - DamageAmount <= 0)
	{
		health = FMath::Min(health - DamageAmount, 0.0f);
	}
	else
	{
		health -= DamageAmount;
	}

	return health;
}

void AMainPlayer::SetMovementStatus(EPlayerMovementStatus status)
{
	movementStatus = status;
	switch (movementStatus)
	{
	case EPlayerMovementStatus::EPMS_Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = sprintingSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
		break;
	}
}

void AMainPlayer::InteractKeyDown()
{
	if (bIsAttacking || GetMovementComponent()->IsFalling())
	{
		return;
	}

	if (overlappingWeapon)
	{
		if (equippedWeapon)
		{
			//不然武器放下会触发碰撞, 覆盖overlappingWeapon, 所以需要临时保存一下
			auto tempVar = overlappingWeapon;
			equippedWeapon->UnEquip(this);
			tempVar->Equip(this);
		}
		else
		{
			overlappingWeapon->Equip(this);
		}
	}
	else
	{
		if (equippedWeapon)
		{
			equippedWeapon->UnEquip(this);
		}
	}
}

void AMainPlayer::EquipWeapon(AWeaponItem* weaponItem)
{
	bHasWeapon = true;
	equippedWeapon = weaponItem;
	overlappingWeapon = nullptr;
}

void AMainPlayer::UnEquipWeapon(AWeaponItem* weaponItem)
{
	bHasWeapon = false;
	equippedWeapon = nullptr;

	if (overlappingWeapon == nullptr)
	{
		overlappingWeapon = weaponItem;
	}
}

void AMainPlayer::AttackKeyDown()
{
	bAttackKeyDown = true;

	if (bHasWeapon)
	{
		Attack();
	}
}

void AMainPlayer::Attack()
{
	if (!bIsAttacking && !GetMovementComponent()->IsFalling())
	{
		bIsAttacking = true;

		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && attackMontage)
		{
			const float playRate = FMath::RandRange(1.25f, 1.75f);
			const FString sectionName = FString::FromInt(FMath::RandRange(1, 2));
			animInstance->Montage_Play(attackMontage, playRate);
			animInstance->Montage_JumpToSection(FName(*sectionName), attackMontage);
		}
	}
}

void AMainPlayer::AttackEnd()
{
	bIsAttacking = false;

	if (bAttackKeyDown)
	{
		AttackKeyDown();
	}
}

void AMainPlayer::UpdateAttackTarget()
{
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors, enemyFilter);

	if (overlappingActors.Num() == 0)
	{
		attackTarget = nullptr;
		return;
	}

	ABaseEnemy* closestEnemy = nullptr;
	float minDistance = FLT_MAX;
	const FVector location = GetActorLocation();

	for (const auto& actor : overlappingActors)
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
		if (enemy && enemy->EnemyMovementStatus != EEnemyMovementStatus::EEMS_Dead)
		{
			float dist = (enemy->GetActorLocation() - location).SizeSquared();
			if (dist < minDistance)
			{
				minDistance = dist;
				closestEnemy = enemy;
			}
		}
	}

	attackTarget = closestEnemy;
}
