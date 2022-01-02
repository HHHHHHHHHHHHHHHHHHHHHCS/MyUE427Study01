// Fill out your copyright notice in the Description page of Project Settings.
#include "TriggerDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyUE427Study01/Characters/Player/MainPlayer.h"

// Sets default values
ATriggerDoor::ATriggerDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(60.0f, 60.0f, 30.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	USceneComponent* root = TriggerBox;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(root);
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(root);
}

// Called when the game starts or when spawned
void ATriggerDoor::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerDoor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerDoor::OnOverlapEnd);
}

// Called every frame
void ATriggerDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATriggerDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
	if (!mainPlayer)
	{
		return;
	}

	OpenDoor();
	LowerTrigger();
}

void ATriggerDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
	if (!mainPlayer)
	{
		return;
	}
	
	CloseDoor();
	RaiseTrigger();
}
