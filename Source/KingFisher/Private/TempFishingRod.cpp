// Fill out your copyright notice in the Description page of Project Settings.


#include "TempFishingRod.h"
#include <Components/SphereComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ATempFishingRod::ATempFishingRod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(compSphere);
	compSphere->SetCollisionProfileName(TEXT("PickUp"));
	compSphere->SetSimulatePhysics(true);

	fishingRod = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FishingRod"));
	fishingRod->SetupAttachment(compSphere);
}

// Called when the game starts or when spawned
void ATempFishingRod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATempFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

