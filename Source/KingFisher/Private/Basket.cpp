// Fill out your copyright notice in the Description page of Project Settings.


#include "Basket.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "Fish.h"
#include "FisherGameModeBase.h"
#include "Bait.h"
#include "FishPlayer.h"
#include "GrabComponent.h"
#include <MotionControllerComponent.h>
#include "FIsherHandMesh.h"
#include "ScoreUIActor.h"

ABasket::ABasket()
{
	PrimaryActorTick.bCanEverTick = true;

	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(compRoot);
	
	compScene = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot Component"));
	compScene->SetupAttachment(compRoot);
	
	basketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Basket"));
	basketMesh->SetupAttachment(compRoot);

	handleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	handleMesh->SetupAttachment(compScene);
	handleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	compBox->SetupAttachment(handleMesh);
	compBox->SetCollisionObjectType(ECC_GameTraceChannel3);

	compFishBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GetFishZone"));
	compFishBox->SetupAttachment(basketMesh);

	scorePos = CreateDefaultSubobject<USceneComponent>(TEXT("Score Widget"));
	scorePos->SetupAttachment(RootComponent);
}

void ABasket::BeginPlay()
{
	Super::BeginPlay();

	compBox->OnComponentBeginOverlap.AddDynamic(this, &ABasket::OnGrab);
	compBox->OnComponentEndOverlap.AddDynamic(this, &ABasket::EndGrab);
	compFishBox->OnComponentBeginOverlap.AddDynamic(this, &ABasket::GetFish);
	compFishBox->OnComponentEndOverlap.AddDynamic(this, &ABasket::LostFish);
}

void ABasket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpen)
	{
		lidLoc = handLoc-player->leftHand->GetComponentLocation();
		lidLocZ = FMath::Clamp(compScene->GetComponentRotation().Roll+lidLoc.Z, -40, 50);
		compScene->SetRelativeRotation(FRotator(0, 0, lidLocZ));
	}

	if (destroyFish != nullptr)
	{
		destroyFish->Destroy();
		destroyFish=nullptr;
	}
}

void ABasket::OnGrab(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
// 	if (OtherActor != this)
// 	{
// 		player = Cast<AFishPlayer>(OtherActor);
// 		
// 		if (OtherActor == player)
// 		{
// 			bGrabReady = true;
// 			UE_LOG(LogTemp, Warning, TEXT("GrabReady"));
// 		}
// 	}
}

void ABasket::EndGrab(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
// 	if (OtherActor != this)
// 	{
// 		player = Cast<AFishPlayer>(OtherActor);
// 
// 		if (OtherActor == player)
// 		{
// 			bGrabReady = false;
// 			UE_LOG(LogTemp, Warning, TEXT("GrabReadyEnd"));
// 		}
// 	}
}

void ABasket::GetFish(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ABait* bait = Cast<ABait>(OtherActor);

		if (OtherActor == bait)
		{
			if (bait->bitingfish != nullptr)
			{
				AGameModeBase* currMode = GetWorld()->GetAuthGameMode();
				gameMode = Cast<AFisherGameModeBase>(currMode);

				gameMode->AddScore(1);
				GetWorld()->SpawnActor<AScoreUIActor>(AScoreUIActor::StaticClass(), scorePos->GetComponentLocation(), scorePos->GetComponentRotation());
				bait->bitingfish->Destroy();
				bait->bitingfish=nullptr;
				bait->Destroy();
			}
		}

// 		AFish* fish = Cast<AFish>(OtherActor);
// 
// 		if (OtherActor == fish)
// 		{
// 			destroyFish = fish;
// 
//  			AGameModeBase* currMode = GetWorld()->GetAuthGameMode();
// 	 		gameMode = Cast<AFisherGameModeBase>(currMode);
// 
// 	 		gameMode->AddScore(1);
//  			GetWorld()->SpawnActor<AScoreUIActor>(AScoreUIActor::StaticClass(), scorePos->GetComponentLocation(), scorePos->GetComponentRotation());
// 		}
	}
}

void ABasket::LostFish(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
// 	if (OtherActor != nullptr)
// 	{
// 		ABait* bait = Cast<ABait>(OtherActor);
// 
// 		if (OtherActor == bait)
// 		{
// 			AGameModeBase* currMode = GetWorld()->GetAuthGameMode();
// 			gameMode = Cast<AFisherGameModeBase>(currMode);
// 
// 			gameMode->AddScore(-1);
// 		}
// 	}
}