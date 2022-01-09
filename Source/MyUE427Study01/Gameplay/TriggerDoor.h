// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerDoor.generated.h"

UCLASS()
class MYUE427STUDY01_API ATriggerDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATriggerDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TriggerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Triggerable Door|Door Properties")
	float DelayTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category="Triggerable Door|Trigger Switch")
	void RaiseTrigger();

	UFUNCTION(BlueprintImplementableEvent, Category="Triggerable Door|Trigger Switch")
	void LowerTrigger();

	UFUNCTION(BlueprintImplementableEvent, Category="Triggerable Door|Door Switch")
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, Category="Triggerable Door|Door Switch")
	void CloseDoor();

	UFUNCTION(BlueprintCallable, Category="Triggerable Door|Trigger Switch")
	void UpdateTriggerLocation(FVector offset);

	UFUNCTION(BlueprintCallable, Category="Triggerable Door|Door Switch")
	void UpdateDoorTrigger(FVector offset);

private :
	FVector initTriggerLocation;
	FVector initDoorLocation;

	FTimerHandle CloseDoorTimerHandle;

	// bool bIsStayTrigger;
};
