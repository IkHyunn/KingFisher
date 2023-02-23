// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FisherGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API AFisherGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFisherGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 minute = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 second = 20;

	int32 currScore = 0;

	bool bCountEnd = false;
	bool bGameEnd = false;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* mainBGM;

public:
	void AddScore(int32 value);
	void CountDown();
};
