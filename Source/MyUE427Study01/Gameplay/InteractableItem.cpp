// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableItem.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AInteractableItem::AInteractableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	DisplayMesh->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
