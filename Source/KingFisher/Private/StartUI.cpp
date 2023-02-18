// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUI.h"
#include <UMG/Public/Components/Button.h>
#include "StartUIActor.h"

void UStartUI::NativeConstruct()
{
	btn_Start->OnPressed.AddDynamic(this, &UStartUI::StartButton);
	btn_Quit->OnPressed.AddDynamic(this, &UStartUI::QuitButton);
}

void UStartUI::StartButton()
{
	startUIActor->GameStart();
}

void UStartUI::QuitButton()
{
	startUIActor->GameQuit();
}