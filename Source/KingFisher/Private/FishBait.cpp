// Fill out your copyright notice in the Description page of Project Settings.

#include "FishBait.h"
#include "Engine/SkeletalMesh.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>

// Sets default values
AFishBait::AFishBait()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(compBox);
	compBox->SetSimulatePhysics(true);

	//SkeltalMeshComponent
	baitMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BAIT"));
	baitMesh ->SetCollisionProfileName(TEXT("LeftPickUp"));
	baitMesh->SetupAttachment(compBox);

//	SkeltalMesh 불러와서 셋팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBait(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/gold_redeye_fish_rig_exp20_SK.gold_redeye_fish_rig_exp20_SK'"));
	if (tempBait.Succeeded())
	{
		baitMesh->SetSkeletalMesh(tempBait.Object);
}


}

// Called when the game starts or when spawned
void AFishBait::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFishBait::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFloat)
	{
		FVector NewLocation = GetActorLocation();
		float deltaHeight = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		NewLocation.Z += deltaHeight * 15;
		runningTime += DeltaTime;

		SetActorLocation(NewLocation);
	}
}

