// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishUIActor.generated.h"

UCLASS()
class KINGFISHER_API AFinishUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* finishWidgetComp;

	class UFinishUI* finishUI;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* caculateSound;

	float currentTime = 0;

	bool caculateSoundStart = false;

public:
	void GameRestart();
	void GoToMain();
	void GameQuit();
};
