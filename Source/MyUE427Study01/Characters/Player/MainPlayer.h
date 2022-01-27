// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class MYUE427STUDY01_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* followCamera;

	//Category 可以省略""  但是只能用于单个词语  不能有空格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float baseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float baseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float staminaConsumeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (ClampMin = 0, ClampMax = 1))
	float exhaustedStaminaRatio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float runningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 sprintingSpeed;
	
private:
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);

	void MoveRight(float value);

	void Turn(float value);

	void LookUp(float value);

	void TurnAtRate(float rate);

	void LookUpAtRate(float rate);

	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float value);

	UFUNCTION(BlueprintCallable)
	void IncreaseStamina(float value);

	UFUNCTION(BlueprintCallable)
	void IncreaseCoin(int value);
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
};
