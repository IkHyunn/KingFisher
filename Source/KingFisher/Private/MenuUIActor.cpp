// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "GameMenuUI.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Overlay.h>

// Sets default values
AMenuUIActor::AMenuUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	menuWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Game Menu Widget Component"));
	SetRootComponent(menuWidgetComp);

 	ConstructorHelpers::FClassFinder<UUserWidget>tempmenu(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/Widget/WG_GameMenuUI.WG_GameMenuUI_C'"));
	if (tempmenu.Succeeded())
	{
		menuWidgetComp->SetWidgetClass(tempmenu.Class);
		menuWidgetComp->SetWidgetSpace(EWidgetSpace::World);
 	}
}

// Called when the game starts or when spawned
void AMenuUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (menuWidgetComp != nullptr)
	{
		menuUI = Cast<UGameMenuUI>(menuWidgetComp->GetWidget());
		if (menuUI != nullptr)
		{
			menuUI->menuUIActor=this;
		}
	}
}

// Called every frame
void AMenuUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMenuUIActor::GameRestart()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestLevel"));
}

void AMenuUIActor::GameSettings()
{
	if (menuUI != nullptr)
	{
		if (!menuUI->bPopup)
		{
//			menuUI->ovr_Settings->SetVisibility(ESlateVisibility::Visible);
			menuUI->PlayAnimation(menuUI->SettingsPopup, 0, 1);
			menuUI->bPopup=true;
		}
		else
		{
//			menuUI->ovr_Settings->SetVisibility(ESlateVisibility::Hidden);
			menuUI->PlayAnimationReverse(menuUI->SettingsPopup, 1);
			menuUI->bPopup=false;
		}
	}
}

void AMenuUIActor::GameResume()
{
	Destroy();
}
