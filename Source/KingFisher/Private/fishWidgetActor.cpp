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

	// ��ũ��
	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	SetRootComponent(screenComp);


	//���� ������Ʈ 
	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);

	//���� 
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

	// GameMode ĳ��
	gameMode = Cast<AFisherGameModeBase>(GetWorld()->GetAuthGameMode());


	if (widgetComp != nullptr)
	{
		ui = Cast<UfishUI>(widgetComp->GetWidget());

		if (ui != nullptr)
		{
			ui->fishUIActor = this;
		}
	}

	// ����� ���� �ʱ�ȭ ****************************
	UpdateFishNumTxt();
}


void AfishWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� 
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

// Release ��ư ������ �� ����Ǵ� �Լ�
void  AfishWidgetActor::ReleaseFish()
{
	// �̳��� �ɸ� ����⸸ �����Ѵ�. *****************
	if (fishfsm->me->GetAttachParentActor() == fishfsm->target)
	{
		fishfsm->me->Destroy();
	}

}



// Keep ������ �� ����Ǵ� �Լ�
void  AfishWidgetActor::KeepFish()
{

	if (ui != nullptr)
	{
		// ********* ������ ********

		//����� ���� ������Ʈ (GameMode���� ���� �����ͼ� ���ε�)
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