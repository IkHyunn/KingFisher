
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


}

void UfishUI::Release()
{
	fishUIActor->ReleaseFish();
}

void UfishUI::Keep()
{
	fishUIActor->KeepFish();
}


