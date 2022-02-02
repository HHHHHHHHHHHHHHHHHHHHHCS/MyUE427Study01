// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

#include"Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "MyUE427Study01/Characters/Player/MainPlayer.h"

AWeaponItem::AWeaponItem()
{
	TriggerVolume->SetSphereRadius(64.0f);

	DisplayMesh->DestroyComponent();
	//因为不知道是不是立马销毁 所以TEXT里面的name要跟之前的不一样, 不然可能会报错
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<USoundCue> soundCueAsset(
		TEXT("SoundCue'/Game/Assets/Audios/Blade_Cue.Blade_Cue'"));
	if (soundCueAsset.Succeeded())
	{
		OnEquippedSound = soundCueAsset.Object;
	}

	bShouldReserveIdleParticle = false;
	WeaponState = EWeaponState::EWS_CanPickup;
}


void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (OtherActor && WeaponState == EWeaponState::EWS_CanPickup)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer)
		{
			mainPlayer->SetOverlappingWeapon(this);
		}
	}
}

void AWeaponItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* mainPlayer = Cast<AMainPlayer>(OtherActor);
		if (mainPlayer && mainPlayer->GetOverlappingWeapon() == this)
		{
			mainPlayer->SetOverlappingWeapon(nullptr);
		}
	}
}


void AWeaponItem::Equip(AMainPlayer* mainPlayer)
{
	if (mainPlayer)
	{
		WeaponState = EWeaponState::EWS_Equipped;

		const USkeletalMeshSocket* rightHandSocket = mainPlayer->GetMesh()->GetSocketByName("RightHandSocket");

		if (rightHandSocket)
		{
			rightHandSocket->AttachActor(this, mainPlayer->GetMesh());
			mainPlayer->EquipWeapon(this);

			bNeedRotate = false;

			if (OnEquippedSound)
			{
				UGameplayStatics::PlaySound2D(this, OnEquippedSound);
			}
			if (!bShouldReserveIdleParticle)
			{
				IdleParticle->Deactivate();
			}
		}
	}
}

void AWeaponItem::UnEquip(AMainPlayer* mainPlayer)
{
}