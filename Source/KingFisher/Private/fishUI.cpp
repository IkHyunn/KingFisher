
#include "fishUI.h"
#include "Components//Button.h"
#include "fishWidgetActor.h"
#include "Fish.h"
#include "Fish_FSM.h"
#include "Kismet/GameplayStatics.h"
#include "FishPlayer.h"

void UfishUI::NativeConstruct()
{
	btn_Release->OnPressed.AddDynamic(this, &UfishUI::Release);
	btn_Keep->OnPressed.AddDynamic(this, &UfishUI::Keep);

	player = Cast <AFishPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

}

void UfishUI::Release()
{
	fishUIActor->ReleaseFish();
}

void UfishUI::Keep()
{
	fishUIActor->KeepFish();
}


