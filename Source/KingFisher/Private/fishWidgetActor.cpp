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



AfishWidgetActor::AfishWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	screenComp->SetupAttachment(RootComponent);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
}


void AfishWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	if (widgetComp != nullptr)
	{
		UfishUI* fishwidget = Cast<UfishUI>(widgetComp->GetWidget());

		if (fishwidget != nullptr)
		{
			fishwidget->fishUIActor = this;
		}
	}

	// ����ġ �ʱ�ȭ
	UpdateExperienceTxt();
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
	//������ �����.
	if (ui)
	{
		//ui->SetVisibility(ESlateVisibility::Collapsed);
		Destroy();
	}

//����ġ �ø���.
// 	if (fish)
// 	{
// 		fish->Experience += 50;
// 		UpdateExperienceTxt();
// 	}
}

// ����ġ �ؽ�Ʈ ������Ʈ �Լ�
void AfishWidgetActor::UpdateExperienceTxt()
{
// 	if (ui->txt_experience && fish)
// 	{
// 		FString ExperienceTxt = FString::Printf(TEXT("%d"), fish->Experience);
// 		ui->txt_experience->SetText(FText::FromString(ExperienceTxt));
// 	}
}
