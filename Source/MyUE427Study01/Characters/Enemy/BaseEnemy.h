// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	void MoveToTarget(class AMainPlayer* targetPlayer);
};
