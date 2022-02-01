// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API AWeaponItem : public AInteractableItem
{
	GENERATED_BODY()

public: 
	AWeaponItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Sond")
	class USoundCue* OnEquippedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Particle")
	bool bShouldReserveIdleParticle;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex) override;

	
protected:
	virtual void BeginPlay() override;

private:
	void Equip();

	void UnEquip();
	
};
