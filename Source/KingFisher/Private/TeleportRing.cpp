// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportRing.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Components/SceneComponent.h>

// Sets default values
ATeleportRing::ATeleportRing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	teleportRingFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportRing"));
	teleportRingFX->SetupAttachment(RootComponent);

	teleportBoundFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportBound"));
	teleportBoundFX->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeleportRing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportRing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

