// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnimInstance.h"

#include "MainPlayer.h"
#include "GameFramework/PawnMovementComponent.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	playerRef = Cast<AMainPlayer>(TryGetPawnOwner());
}

void UMainPlayerAnimInstance::UpdateAnimationProperties()
{
	if (playerRef == nullptr)
	{
		playerRef = Cast<AMainPlayer>(TryGetPawnOwner());
	}

	if(playerRef)
	{
		const FVector speedVec = playerRef->GetVelocity();
		const FVector planeSpeed = FVector(speedVec.X, speedVec.Y, 0.0f);

		speed = planeSpeed.Size();

		bIsInAir = playerRef->GetMovementComponent()->IsFalling();
	}
}
