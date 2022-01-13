// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyUE427Study01GameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AMyUE427Study01GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIClass)
	{
		MainUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), MainUIClass);
		if(MainUI)
		{
			MainUI->AddToViewport();
		}
	}
}
