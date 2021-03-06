// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementStatus: uint8
{
	EPMS_Normal UMETA(Displayname = "Normal"),
	EPMS_Sprinting UMETA(Displayname = "Sprinting"),
	EPMS_Dead UMETA(Displayname = "Dead"),
};

UENUM(BlueprintType)
enum class EPlayerStaminaStatus: uint8
{
	EPSS_Normal UMETA(Displayname = "Normal"),
	EPSS_Exhausted UMETA(Displayname = "Exhausted"),
	EPSS_ExhaustedRecovering UMETA(Displayname = "ExhaustedRecovering"),
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerStaminaStatus staminaStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float runningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float sprintingSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerMovementStatus movementStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* attackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AWeaponItem* equippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	TSubclassOf<class ABaseEnemy> enemyFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	float interpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hit Effect")
	class UParticleSystem* hitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hit Effect")
	class USoundCue* hitSound;

private:
	bool bLeftShiftKeyDown;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasWeapon;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	AWeaponItem* overlappingWeapon;

	bool bAttackKeyDown;

	bool bIsAttacking;

	ABaseEnemy* attackTarget;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Attack")
	bool bInterpToEnemy;

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

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	FORCEINLINE void LeftShiftKeyDown() { bLeftShiftKeyDown = true; }

	FORCEINLINE void LeftShiftKeyUp() { bLeftShiftKeyDown = false; }

	void SetMovementStatus(EPlayerMovementStatus status);

	FORCEINLINE void SetOverlappingWeapon(AWeaponItem* weaponItem) { overlappingWeapon = weaponItem; }

	FORCEINLINE AWeaponItem* GetOverlappingWeapon() { return overlappingWeapon; }

	void InteractKeyDown();

	void EquipWeapon(AWeaponItem* weaponItem);

	void UnEquipWeapon(AWeaponItem* weaponItem);

	//如果写const 蓝图读取 类型会报错 莫名其妙
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetHasWeapon() { return bHasWeapon; }

	void AttackKeyDown();

	FORCEINLINE void AttackKeyUp() { bAttackKeyDown = false; };

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void UpdateAttackTarget();

	FORCEINLINE bool IsAlive() const { return movementStatus != EPlayerMovementStatus::EPMS_Dead; }

	void ReStartLevel() const;
};
