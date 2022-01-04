// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainPlayer::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	PlayerInputComponent->BindAxis("TurnAtRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AMainPlayer::LookUpAtRate);
}

void AMainPlayer::MoveForward(float value)
{
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
	Super::Jump();
}
