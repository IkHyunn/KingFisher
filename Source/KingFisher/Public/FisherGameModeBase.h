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
	int32 currScore = 0;

public:
	void AddScore(int32 value);
};
