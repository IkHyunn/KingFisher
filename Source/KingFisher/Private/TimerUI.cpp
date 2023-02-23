// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerUI.h"
#include "FisherGameModeBase.h"
#include <UMG/Public/Components/TextBlock.h>
#include "FishPlayer.h"
#include <Kismet/GameplayStatics.h>

void UTimerUI::NativeConstruct()
{
	Super::NativeConstruct();

	currMode = Cast<AFisherGameModeBase>(GetWorld()->GetAuthGameMode());

	if (currMode != nullptr)
	{
		text_Minutes->SetText(FText::AsNumber(currMode->minute));
		text_Seconds->SetText(FText::AsNumber(currMode->second));
	}

	player = Cast<AFishPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UTimerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (currMode != nullptr)
	{
		text_Minutes->SetText(FText::AsNumber(currMode->minute));
		text_Seconds->SetText(FText::AsNumber(currMode->second));

		if (currMode->minute == 0 && currMode->second <= 30)
		{
			currentTime += InDeltaTime;
			if (currentTime > delayTime)
			{
				if (currMode->second > 0)
				{
					TimeWarning();
				}
				currentTime = 0;
			}
		}
	}

// 	if (currMode->minute == 0 && currMode->second == 0)
// 	{
// 		player->OpenFinishUI();
// 	}
}

void UTimerUI::TimeWarning()
{
	PlayAnimation(TimeAlarm, 0.0f, 1);
}