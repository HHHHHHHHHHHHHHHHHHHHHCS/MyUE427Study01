// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveItem.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "MyUE427Study01/Characters/Enemy/BaseEnemy.h"
#include "MyUE427Study01/Characters/Player/MainPlayer.h"

AExplosiveItem::AExplosiveItem()
{
	TriggerVolume->SetSphereRadius(50.0f);
	Damage = 20.0f;
}

void AExplosiveItem::OnPickOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	Super::OnPickOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		const AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		const ABaseEnemy* baseEnemy = Cast<ABaseEnemy>(OtherActor);
		if (mainPlayer || baseEnemy)
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
			TSet<AActor*> mainplayers;
			GetOverlappingActors(mainplayers, TSubclassOf<AMainPlayer>{});

			for (auto& actor : mainplayers)
			{
				UGameplayStatics::ApplyDamage(actor, Damage, nullptr, this, DamageTypeClass);
			}
			
			TSet<AActor*> enemys;
			GetOverlappingActors(mainplayers, TSubclassOf<ABaseEnemy>{});

			for (auto& actor : enemys)
			{
				UGameplayStatics::ApplyDamage(actor, Damage, nullptr, this, DamageTypeClass);
			}

			Destroy();
		}
	}
}

void AExplosiveItem::OnPickOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
