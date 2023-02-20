// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreUIActor.h"
#include <UMG/Public/Components/WidgetComponent.h>

// Sets default values
AScoreUIActor::AScoreUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	plusScoreComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Plus Score Widget"));
	SetRootComponent(plusScoreComp);

	ConstructorHelpers::FClassFinder<UUserWidget>tempscore(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/Widget/WG_ScoreUI.WG_ScoreUI_C'"));
	if (tempscore.Succeeded())
	{
		plusScoreComp->SetWidgetClass(tempscore.Class);
		plusScoreComp->SetWidgetSpace(EWidgetSpace::World);
	}
}

// Called when the game starts or when spawned
void AScoreUIActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScoreUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if (currentTime > delayTime)
	{
		Destroy();
		currentTime = 0;
	}
}

