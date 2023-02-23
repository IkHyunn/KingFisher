// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <Kismet/GameplayStatics.h>
#include "FinishUI.h"
#include <Components/AudioComponent.h>
#include "FisherGameModeBase.h"

// Sets default values
AFinishUIActor::AFinishUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	finishWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Finish Widget Component"));
	SetRootComponent(finishWidgetComp);

	caculateSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Caculate Sound Component"));
	caculateSound->bAutoActivate = false;
	caculateSound->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFinishUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (finishWidgetComp != nullptr)
	{
		finishUI = Cast<UFinishUI>(finishWidgetComp->GetWidget());

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

		currentTime += DeltaTime;

		if (currentTime > 1.5)
		{
			if (!caculateSoundStart)
			{
				if(!finishUI->bIncreasingEnd)
				{
					caculateSound->Play();
					caculateSoundStart = true;
				}
			}
			currentTime = 0;
		}

		if (caculateSoundStart)
		{
			if (finishUI->bIncreasingEnd)
			{
				caculateSound->Stop();
			}
		}
} 

void AFinishUIActor::GameRestart()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("map_village_day"));
}

void AFinishUIActor::GoToMain()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
}

void AFinishUIActor::GameQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}