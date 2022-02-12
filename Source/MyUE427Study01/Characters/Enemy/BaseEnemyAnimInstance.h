// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
	float speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
	class ABaseEnemy* baseEnemyRef;

public :
	virtual void NativeInitializeAnimation() override;


	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();
};
