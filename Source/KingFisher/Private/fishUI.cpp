
#include "fishUI.h"
#include "Components//Button.h"
#include "fishWidgetActor.h"
#include "Fish.h"
#include "Fish_FSM.h"

void UfishUI::NativeConstruct()
{
	btn_Release->OnPressed.AddDynamic(this, &UfishUI::Release);
	btn_Keep->OnPressed.AddDynamic(this, &UfishUI::Keep);

	//**********Ãß°¡ ***********
// 	if (fish)
// 	{
// 		fish->OnFishDeath().AddDynamic(this, &UfishUI::OnFishDeathHandler);
// 	}
}

void UfishUI::Release()
{
	fishUIActor->ReleaseFish();
}

void UfishUI::Keep()
{
	fishUIActor->KeepFish();
}




// void UfishUI::OnFishDeathHandler()
// {
// 	//UI ¶ç¿ì±â
// 	if (fishfsm->bIsDead)
// 	{
// 		UfishUI* fishUI = CreateWidget<UfishUI>(GetWorld(), fishfsm->bIsDead);
// 		if (fishUI)
// 		{
// 			fishUI->AddToViewport();
// 		}
// 	}
// }

