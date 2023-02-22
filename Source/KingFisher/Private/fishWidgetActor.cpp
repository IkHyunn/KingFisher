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

	// ����� ���� �ʱ�ȭ ****************************
	UpdateFishNumTxt();
}


void AfishWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	FString FishNumtxt = FString::Printf(TEXT("%d"), gameMode->currScore);
	ui->txt_experience->SetText(FText::FromString(FishNumtxt));
		
	
}

