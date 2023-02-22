// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterSurface.h"
#include <Components/BoxComponent.h>
#include "FishBait.h"
#include "FishingRod.h"
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AWaterSurface::AWaterSurface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Surface Collision"));
	SetRootComponent(compBox);

	ConstructorHelpers::FObjectFinder<USoundCue>tempbaitsplash(TEXT("/Script/Engine.SoundCue'/Game/Resources/Sound/sc_FOL_BaitSplash.sc_FOL_BaitSplash'"));
	if (tempbaitsplash.Succeeded())
	{
		sc_BaitSplash = tempbaitsplash.Object;
	}
}

// Called when the game starts or when spawned
void AWaterSurface::BeginPlay()
{
	Super::BeginPlay();
	
	compBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterSurface::OnWaterSurface);
}

// Called every frame
void AWaterSurface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterSurface::OnWaterSurface(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		AFishingRod* fishingRod = Cast<AFishingRod>(OtherActor);

 		if (OtherComp == fishingRod->bobberMesh)
 		{
			fishingRod->bobberMesh->SetSimulatePhysics(false);
			fishingRod->bobberMesh->SetWorldRotation(FRotator(0));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), sc_BaitSplash, fishingRod->bobberMesh->GetComponentLocation());
			fishingRod->bBobberFloat = true;
			UE_LOG(LogTemp,Warning,TEXT("Bobber Overlapped"));
		}
	}
}