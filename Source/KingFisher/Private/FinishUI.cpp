// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishUI.h"
#include "FisherGameModeBase.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>

void UFinishUI::NativeConstruct()
{
	Super::NativeConstruct();

	currMode = Cast<AFisherGameModeBase>(GetWorld()->GetAuthGameMode());
	
	fishScore->SetText(FText::AsNumber(finalScore));

	btn_Restart->OnPressed.AddDynamic(this, &UFinishUI::RestartButton);
	btn_Main->OnPressed.AddDynamic(this, &UFinishUI::MainButton);
	btn_Quit->OnPressed.AddDynamic(this, &UFinishUI::QuitButton);
}

void UFinishUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIncreasingEnd)
	{
		currentTime += InDeltaTime;

		if (currentTime > 1.5)
		{
			if (finalScore < currMode->currScore)
			{
				finalScore += finalScore + 1;
			}
			else
			{
				finalScore = currMode->currScore;
				bIncreasingEnd = true;
				currentTime = 0;
			}

			fishScore->SetText(FText::AsNumber(finalScore));
		}
	}
}

void UFinishUI::RestartButton()
{
	finishUIActor->GameRestart();
}

void UFinishUI::MainButton()
{
	finishUIActor->GoToMain();
}

void UFinishUI::QuitButton()
{
	finishUIActor->GameQuit();
}