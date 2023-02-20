// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuUI.h"
#include "MenuUIActor.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/Overlay.h>
#include <Kismet/GameplayStatics.h>
#include "FishPlayer.h"
#include <UMG/Public/Components/WidgetComponent.h>

void UGameMenuUI::NativeConstruct()
{
	btn_Restart->OnPressed.AddDynamic(this, &UGameMenuUI::RestartButton);
	btn_Settings->OnPressed.AddDynamic(this, &UGameMenuUI::SettingsButton);
	btn_Resume->OnPressed.AddDynamic(this, &UGameMenuUI::ResumeButton);

	player = Cast<AFishPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UGameMenuUI::RestartButton()
{	
//	menuUIActor->GameRestart();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
}

void UGameMenuUI::SettingsButton()
{
//	menuUIActor->GameSettings();
	if (!bPopup)
	{
//		ovr_Settings->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(SettingsPopup, 0, 1);
		bPopup = true;
	}
	else
	{
//		ovr_Settings->SetVisibility(ESlateVisibility::Hidden);
		PlayAnimationReverse(SettingsPopup, 1);
		bPopup = false;
	}
}

void UGameMenuUI::ResumeButton()
{
//	menuUIActor->GameResume();
	player->menuWidgetComp->SetVisibility(false);
}