// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <Kismet/GameplayStatics.h>
#include "FinishUI.h"

// Sets default values
AFinishUIActor::AFinishUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	finishWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Finish Widget Component"));
	SetRootComponent(finishWidgetComp);
}

// Called when the game starts or when spawned
void AFinishUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (finishWidgetComp != nullptr)
	{
		UFinishUI* finishUI = Cast<UFinishUI>(finishWidgetComp->GetWidget());
		if (finishUI != nullptr)
		{
			finishUI->finishUIActor = this;
		}
	}
}

// Called every frame
void AFinishUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishUIActor::GameRestart()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("TestLevel"));
}

void AFinishUIActor::GoToMain()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
}

void AFinishUIActor::GameQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}