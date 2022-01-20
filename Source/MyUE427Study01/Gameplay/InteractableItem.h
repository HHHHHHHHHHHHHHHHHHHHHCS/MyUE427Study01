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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* IdleParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable Item|Particles")
	class UParticleSystemComponent* OverlapParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable Item|Sounds")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable Item|ItemProperties")
	bool bNeedRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable Item|Item Properties")
	float RotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
};