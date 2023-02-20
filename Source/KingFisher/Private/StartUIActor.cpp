// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "StartUI.h"
#include <Kismet/KismetSystemLibrary.h>
#include "FishPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/EngineTypes.h>

// Sets default values
AStartUIActor::AStartUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	startWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Start Widget Component"));
	startWidgetComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStartUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (startWidgetComp != nullptr)
	{
		UStartUI* startUI = Cast<UStartUI>(startWidgetComp->GetWidget());
		if (startUI != nullptr)
		{
			startUI->startUIActor=this;
		}
	}

	player = Cast<AFishPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AStartUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bGameStart)
	{
		currentTime += DeltaTime;
		if (currentTime > delayTime)
		{
			OpenMainLevel();
			currentTime = 0;
		}
	}
}

void AStartUIActor::GameStart()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 1, 1, FLinearColor::Black, false, true);
	bGameStart = true;
// 	FTimerHandle fadeTimer;
// 	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this, &AStartUIActor::OpenMainLevel, 1.0f, false);
}

void AStartUIActor::GameQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AStartUIActor::OpenMainLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("map_village_day"));
}
