// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem.h"
#include "ExplosiveItem.generated.h"

/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API AExplosiveItem : public AInteractableItem
{
	GENERATED_BODY()

public:
	AExplosiveItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damge")
	float Damage;

public:

	
public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex) override;
};
