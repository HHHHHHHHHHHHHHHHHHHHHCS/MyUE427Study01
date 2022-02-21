// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	baseEnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
}

void UBaseEnemyAnimInstance::UpdateAnimationProperties()
{
	if (baseEnemyRef == nullptr)
	{
		baseEnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
	}

	if (baseEnemyRef)
	{
		const FVector speedVec = baseEnemyRef->GetVelocity();
		const FVector planeSpeed = FVector(speedVec.X, speedVec.Y, 0.0f);

		speed = planeSpeed.Size();

		// float direction = CalculateDirection(baseEnemyRef->GetVelocity(), baseEnemyRef->GetActorRotation());
	}
}
