// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableItem.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AInteractableItem::AInteractableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(GetRootComponent());

	bNeedRotate = true;
	RotationRate = 45.0f;
}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractableItem::OnPickOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractableItem::OnPickOverlapEnd);
}

// Called every frame
void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bNeedRotate)
	{
		FRotator newRotation = GetActorRotation();
		newRotation.Yaw += RotationRate * DeltaTime;
		SetActorRotation(newRotation);
	}
}

void AInteractableItem::OnPickOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
}

void AInteractableItem::OnPickOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
