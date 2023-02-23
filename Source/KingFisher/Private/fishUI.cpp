
#include "fishUI.h"
#include "Components//Button.h"
#include "fishWidgetActor.h"
#include "Fish.h"
#include "Fish_FSM.h"
#include "Kismet/GameplayStatics.h"
#include "FishPlayer.h"
#include "FisherGameModeBase.h"
#include "UMG/Public/Components/TextBlock.h"

void UfishUI::NativeConstruct()
{
	btn_Release->OnPressed.AddDynamic(this, &UfishUI::Release);
	btn_Keep->OnPressed.AddDynamic(this, &UfishUI::Keep);

	player = Cast <AFishPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

}

void UfishUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (currMode != nullptr)
	{
		if (!bPopUpEnd)
		{
			if (Score == currMode->currScore)
			{
				Score = currMode->currScore;
				bPopUpEnd = true;
				currTime = 0;
			}

			if (Score < currMode->currScore)
			{
				currTime += InDeltaTime;

				if (currTime > 1.5)
				{
					Score += Score + 1;

					txt_experience->SetText(FText::AsNumber(Score));
				}
			}
			else
			{
				Score = currMode->currScore;
				bPopUpEnd = true;
				currTime = 0;
			}
		}
	}
}

void UfishUI::Release()
{
	fishUIActor->ReleaseFish();
}

void UfishUI::Keep()
{
	fishUIActor->KeepFish();
}


