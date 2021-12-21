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
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
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

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
}

void AMainPlayer::MoveForward(float value)
{
	if (Controller == nullptr || value == 0)
	{
		return;
	}

	// AddMovementInput(GetActorForwardVector(), value);
	FRotator rotation = Controller->GetControlRotation();
	FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
	FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, value);
}

void AMainPlayer::MoveRight(float value)
{
	if (Controller == nullptr || value == 0)
	{
		return;
	}
	
	FRotator rotation = Controller->GetControlRotation();
	FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
	FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, value);
}
