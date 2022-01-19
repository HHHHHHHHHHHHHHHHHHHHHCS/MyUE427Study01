// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableItem.generated.h"

UCLASS()
class MYUE427STUDY01_API AInteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* DisplayMesh;

	class UParticleSystemComponent* IdleParticle;

	class UParticleSystemComponent* OverlapParticle;

	class USoundCue* OverlapSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
