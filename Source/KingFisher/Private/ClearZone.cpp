// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearZone.h"
#include <Components/BoxComponent.h>
#include "FishPlayer.h"
#include "FisherGameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AClearZone::AClearZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Clear Zone"));
	SetRootComponent(compBox);
	compBox->SetBoxExtent(FVector(50));
}

// Called when the game starts or when spawned
void AClearZone::BeginPlay()
{
	Super::BeginPlay();
	
	compBox->OnComponentBeginOverlap.AddDynamic(this, &AClearZone::StageClear);
}

// Called every frame
void AClearZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AClearZone::StageClear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFisherGameModeBase* currMode = GetWorld()->GetAuthGameMode<AFisherGameModeBase>();
	AFishPlayer* player = Cast<AFishPlayer>(OtherActor);

	if (OtherActor == player)
	{
		if (currMode->currScore >= 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Clear"));
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("map_village_day"));
		}
	}
}

