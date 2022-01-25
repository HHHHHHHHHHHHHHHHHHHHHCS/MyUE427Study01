// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem.h"

#include "MyUE427Study01/Characters/Player/MainPlayer.h"

#include "PickupItem.generated.h"


/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API APickupItem : public AInteractableItem
{
	GENERATED_BODY()

public:
	APickupItem();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex) override;


	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickup(AMainPlayer* mainPlayer);
};
