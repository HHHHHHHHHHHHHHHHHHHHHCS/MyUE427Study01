// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"


APickupItem::APickupItem()
{
	TriggerVolume->SetSphereRadius(50.0f);
}

void APickupItem::OnPickOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	Super::OnPickOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			if (OverlapParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticle, GetActorLocation()
				                                         , FRotator::ZeroRotator, FVector::OneVector, true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			OnPickup(mainPlayer);
			Destroy();
		}
	}
}

void APickupItem::OnPickOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
