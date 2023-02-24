#include "fishWidgetActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "fishUI.h"
#include "Fish_FSM.h"
#include "Fish.h"
#include "Bait.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "FisherGameModeBase.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"



AfishWidgetActor::AfishWidgetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스크린
	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	SetRootComponent(screenComp);


	//위젯 컴포넌트 
	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);

	//사운드 
	popupsound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Component"));
	popupsound->bAutoActivate = false;
	popupsound->SetupAttachment(RootComponent);

	// 	ConstructorHelpers::FClassFinder<UUserWidget> tempui(TEXT(""));
	// 	if (tempui.Succeeded())
	// 	{
	// 		widgetComp->SetWidgetClass(tempui.Class);
	// 		widgetComp->SetWidgetSpace(EWidgetSpace::World);
	// 	}
}


void AfishWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	// GameMode 캐싱
	gameMode = Cast<AFisherGameModeBase>(GetWorld()->GetAuthGameMode());


	if (widgetComp != nullptr)
	{
		ui = Cast<UfishUI>(widgetComp->GetWidget());

		if (ui != nullptr)
		{
			ui->fishUIActor = this;
		}
	}

	// 물고기 개수 초기화 ****************************
	UpdateFishNumTxt();
}


void AfishWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 사운드 
	currentTime += DeltaTime;

	if (currentTime > 1.5)
	{
		if (!soundStart)
		{
			if (!ui->bPopUpEnd)
			{
				popupsound->Play();
				soundStart = true;
			}
		}
		currentTime = 0;
	}

	if (soundStart)
	{
		if (ui->bPopUpEnd)
		{
			popupsound->Stop();
		}
	}

}

// Release 버튼 눌렀을 때 실행되는 함수
void  AfishWidgetActor::ReleaseFish()
{
	// 미끼에 걸린 물고기만 제거한다. *****************
	if (fishfsm->me->GetAttachParentActor() == fishfsm->target)
	{
		fishfsm->me->Destroy();
	}

}



// Keep 눌렀을 때 실행되는 함수
void  AfishWidgetActor::KeepFish()
{

	if (ui != nullptr)
	{
		// ********* 진행중 ********

		//물고기 숫자 업데이트 (GameMode에서 점수 가져와서 바인딩)
		if (fish && ui->txt_experience)
		{
			UpdateFishNumTxt();

			Destroy();
		}
	}

}

void AfishWidgetActor::UpdateFishNumTxt()
{
	if (gameMode != nullptr)
	{
		if (currentTime > 1.5)
		{
			Score = gameMode->currScore;
			ui->txt_experience->SetText(FText::AsNumber(Score));
		}

	}
}