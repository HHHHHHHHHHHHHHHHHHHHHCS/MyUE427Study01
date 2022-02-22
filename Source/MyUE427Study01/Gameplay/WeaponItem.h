// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem.h"
#include "WeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponState: uint8
{
	EWS_CanPickup UMETA(DisplayName = "CanPickup"),
	EWS_Equipped UMETA(DisplayName = "Equpped")
};

/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API AWeaponItem : public AInteractableItem
{
	GENERATED_BODY()
public:
	AWeaponItem();

private:
	FRotator resetRotator;
	FVector resetScale;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Sond")
	class USoundCue* OnEquippedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Particle")
	bool bShouldReserveIdleParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon|Attack")
	class UBoxComponent* attackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapom|Attack")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapom|Attack")
	TSubclassOf<UDamageType> damageTypeClass;

private:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Attack")
	class AController* weaponOwner;

	TSet<class ABaseEnemy*> damagedArray;
	
public:
	virtual void OnPickOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp,
	                                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnPickOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex) override;

	void Equip(class AMainPlayer* mainPlayer);

	void UnEquip(AMainPlayer* mainPlayer);

	UFUNCTION()
	virtual void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp,
	                                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                         UPrimitiveComponent* OtherComp,
	                                         int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActiveAttackCollision();

	UFUNCTION(BlueprintCallable)
	void DeactiveAttackCollision();

protected:
	virtual void BeginPlay() override;

private:
	void ActiveDisplayMeshCollision();

	void DeactiveDisplayMeshCollision();
};
