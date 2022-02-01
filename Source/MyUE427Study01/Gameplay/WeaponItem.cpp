// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

#include"Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

AWeaponItem::AWeaponItem()
{
	TriggerVolume->SetSphereRadius(64.0f);
	
	DisplayMesh->DestroyComponent();
	//因为不知道是不是立马销毁 所以TEXT里面的name要跟之前的不一样, 不然可能会报错
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());
}


void AWeaponItem::BeginPlay()
{
	
}

void AWeaponItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AWeaponItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


void AWeaponItem::Equip()
{
}

void AWeaponItem::UnEquip()
{
}
