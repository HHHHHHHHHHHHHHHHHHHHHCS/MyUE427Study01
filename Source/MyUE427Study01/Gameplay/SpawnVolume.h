// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class MYUE427STUDY01_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Volume")
	TArray<TSubclassOf<AActor>> SpawnActorClassArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Spawn Volume")
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure, Category="Spawn Volume")
	TSubclassOf<AActor> GetSpawnActorClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Spawn Volume")
	void SpawnActor(UClass* spawnClass, FVector spawnLocation);
};
