// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawanBox"));

	RootComponent = SpawnBox;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetSpawnPoint()
{
	const FVector origin = SpawnBox->GetComponentLocation();
	const FVector extent = SpawnBox->GetScaledBoxExtent();

	return UKismetMathLibrary::RandomPointInBoundingBox(origin, extent);
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActorClass()
{
	if (SpawnActorClassArray.Num() > 0)
	{
		const int index = FMath::RandRange(0, SpawnActorClassArray.Num() - 1);
		return SpawnActorClassArray[index];
	}
	else
	{
		return nullptr;
	}

	return TSubclassOf<AActor>();
}
