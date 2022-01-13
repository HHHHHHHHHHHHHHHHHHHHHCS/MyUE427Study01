// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyUE427Study01GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYUE427STUDY01_API AMyUE427Study01GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Widgets")
	TSubclassOf<class UUserWidget> MainUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI Widgets")
	UUserWidget* MainUI;

protected:
	virtual void BeginPlay() override;
};
