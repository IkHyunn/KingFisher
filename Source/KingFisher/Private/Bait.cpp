// Fill out your copyright notice in the Description page of Project Settings.



#include "Bait.h"
#include <Components/SphereComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/BoxComponent.h>
#include "Components/CapsuleComponent.h"
#include "Fish_FSM.h"
#include "Fish.h"

ABait::ABait()
{
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(compBox);
	compBox->SetCollisionObjectType(ECC_GameTraceChannel3);
	compBox->SetSimulatePhysics(true);

	//콜리전 박스
	BaitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BAIT Collision"));
	BaitCollision->SetupAttachment(RootComponent);
	BaitCollision->SetCollisionProfileName(TEXT("PickUp"));
	BaitCollision->SetSimulatePhysics(true);

	baitMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait"));
	baitMesh->SetupAttachment(compBox);
	baitMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//물고기와 오버랩 되었을 때
void ABait::NotifyActorBeginOverlap(AActor* OtherActor)
{
	fish = Cast<AFish>(OtherActor);
	if (fish != nullptr)
	{
		fish->bBait = true;
		//fish->fsm->BiteFunction();
	
	}
}

void ABait::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 물고기 상호작용
	if (bBait)
	{
		currenttime += DeltaTime;
		if (currenttime > 1)
		{
			if (fish->fsm->currHP > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("BAITTT"));
				fish->fsm->ReceiveBait();

			}
			currenttime = 0;
		}
	}
	else
	{
		bBaitReady = true;
		fish = nullptr;
	}
}