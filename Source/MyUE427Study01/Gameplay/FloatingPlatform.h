// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class MYUE427STUDY01_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, Category="Floating Platform")
	FVector StartPoint;

	UPROPERTY(EditAnywhere, Category="Floating Platform", meta=(MakeEditWidget = "true"))
	FVector EndPoint;

	UPROPERTY(EditAnywhere, Category="Floating Platform")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, Category="Floating Platform")
	float DelayTime;

	
private:
	bool bInterping;

	float Distance;
	FTimerHandle InterpTimerHandle;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
