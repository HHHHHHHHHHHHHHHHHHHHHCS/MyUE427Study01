// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Character.h"
#include "MyUE427Study01/Characters/Player/MainPlayer.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus: uint8
{
	EEMS_Idle UMETA(Displayname = "Idle"),
	EEMS_MoveToTarget UMETA(Displayname = "MoveToTarget"),
	EEMS_Attacking UMETA(Displayname = "Attacking"),
	EEMS_Dead UMETA(Displayname = "Dead"),
};

UCLASS()
class MYUE427STUDY01_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class USphereComponent* ChaseVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	USphereComponent* AttackVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Enemy Stats")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Stats")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Stats")
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Enemy Stats")
	class UWidgetComponent* healthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	bool bAttackVolumeOverlapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	class UAnimMontage* attackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	float interpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Attack")
	bool bInterpToPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Attack")
	class UBoxComponent* leftAttackCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Attack")
	UBoxComponent* rightAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	TSubclassOf<UDamageType> damageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hit Effect")
	class UParticleSystem* hitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hit Effect")
	class USoundCue* hitSound;


private:
	class UProgressBar* healthBar;
	TSet<AMainPlayer*> damagedSet;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                       const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                        const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnLeftAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                               UPrimitiveComponent* OtherComp,
	                                               int32 OtherBodyIndex, bool bFromSweep,
	                                               const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnLeftAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                             UPrimitiveComponent* OtherComp,
	                                             int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnRightAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                UPrimitiveComponent* OtherComp,
	                                                int32 OtherBodyIndex, bool bFromSweep,
	                                                const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnRightAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                              UPrimitiveComponent* OtherComp,
	                                              int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainPlayer* targetPlayer);

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void ActiveLeftAttackCollision();

	UFUNCTION(BlueprintCallable)
	void DeactiveLeftAttackCollision();

	UFUNCTION(BlueprintCallable)
	void ActiveRightAttackCollision();

	UFUNCTION(BlueprintCallable)
	void DeactiveRightAttackCollision();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	FORCEINLINE bool IsAlive() const { return EnemyMovementStatus != EEnemyMovementStatus::EEMS_Dead; }

	FORCEINLINE bool HasValidTarget();
};
