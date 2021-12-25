// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API UMainPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
	float speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
	class AMainPlayer* playerRef;

public :
	virtual void NativeInitializeAnimation() override;

	
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();
};
