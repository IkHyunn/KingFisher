// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "fishWidgetActor.generated.h"

UCLASS()
class KINGFISHER_API AfishWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AfishWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* screenComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere)
	class UFish_FSM* fishfsm;

	UPROPERTY(EditAnywhere)
	class AFish* fish;

	UPROPERTY(EditAnywhere)
	class UfishUI* ui; 

	UPROPERTY(EditAnywhere)
	class AFisherGameModeBase* gameMode;

	int32 Score;

	bool bPopUpEnd = false;

	// »ç¿îµå
	UPROPERTY(EditAnywhere)
	class UAudioComponent* popupsound;

	float currentTime = 0;
	bool soundStart = false;


	void ReleaseFish();
	void KeepFish();
	void UpdateFishNumTxt();

};
