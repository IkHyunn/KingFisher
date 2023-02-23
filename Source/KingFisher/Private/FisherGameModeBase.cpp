// Fill out your copyright notice in the Description page of Project Settings.


#include "FisherGameModeBase.h"
#include "FishPlayer.h"
#include <Kismet/GameplayStatics.h>

#include <Components/AudioComponent.h>

AFisherGameModeBase::AFisherGameModeBase()
{
	mainBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("Main BGM Component"));
	mainBGM->SetupAttachment(RootComponent);
}


void AFisherGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	mainBGM->bAutoActivate=true;

	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AFisherGameModeBase::CountDown, 1.0f, true, 0.0f);
}

void AFisherGameModeBase::AddScore(int32 value)
{
	currScore += value;
	UE_LOG(LogTemp, Warning, TEXT("%d"), currScore);
}

void AFisherGameModeBase::CountDown()
{
	if (!bCountEnd)
	{
		if (second != 0)
		{
			second = second - 1;
		}
		else
		{
			if (minute == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Over"));
				minute = 0;
				second = 0;
				bCountEnd = true;
			}
			else
			{
				minute = minute - 1;
				second = 59;
			}
		}
	}

	if (bGameEnd)
	{
		mainBGM->FadeOut(0.5f, 0);
	}
}