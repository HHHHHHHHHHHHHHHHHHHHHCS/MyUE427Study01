// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	StartPoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;
	InterpSpeed = 200.0f;
	DelayTime = 2.0f;
	bInterping = true;
}


// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	Distance = EndPoint.Size();
	EndPoint += StartPoint;
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		const FVector currentLocation = GetActorLocation();
		const FVector newLocation = FMath::VInterpConstantTo(currentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(newLocation);

		const float nowDistance = (GetActorLocation() - StartPoint).Size();
		if (Distance - nowDistance <= 0.1f)
		{
			bInterping = false;
			
			const auto toggleInterpState = [this]()
			{
				bInterping = true;
			};
			GetWorldTimerManager().SetTimer(InterpTimerHandle, FTimerDelegate::CreateLambda(toggleInterpState)
			                                , DelayTime, false);
			
			const FVector tempVec = EndPoint;
			EndPoint = StartPoint;
			StartPoint = tempVec;
		}
	}
}
