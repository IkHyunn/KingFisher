// Fill out your copyright notice in the Description page of Project Settings.


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



AfishWidgetActor::AfishWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	screenComp->SetupAttachment(RootComponent);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UUserWidget> tempui(TEXT(""));
	if (tempui.Succeeded())
	{
		widgetComp->SetWidgetClass(tempui.Class);
		widgetComp->SetWidgetSpace(EWidgetSpace::World);
	}
}


void AfishWidgetActor::BeginPlay()
{
	Super::BeginPlay();

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

	FString FishNumtxt = FString::Printf(TEXT("%d"), gameMode->currScore);
	ui->txt_experience->SetText(FText::FromString(FishNumtxt));
		
	
}

