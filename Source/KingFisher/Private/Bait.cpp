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
		if (bitingfish == nullptr)
		{
			fish->bBait = true;
			bitingfish = fish;

		}

	}
}

void ABait::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 물고기 상호작용
	if (bitingfish != nullptr && bitingfish->bBait)
	{
		if (fish != bitingfish)
		{
			fish->fsm->UpdateReturnPos();

		}

		if (bitingfish->fsm == nullptr)
		{
			return;
		}

		currenttime += DeltaTime;

		if (currenttime > 3)
		{

			if (bitingfish->fsm->currHP > 0)
			{
				if (bitingfish->fsm->currState == EFishState::Swim || bitingfish->fsm->currState == EFishState::FastSwim)
				{
					UE_LOG(LogTemp, Warning, TEXT("BAITTT"));
					bitingfish->fsm->ReceiveBait();

					currenttime = 0;
				}
			}

		}
		else
		{

			// 또 다른 if조건으로 바꿔준다.
			bBaitReady = true;
			fish = nullptr;
		}
	}
}


// if (bitingfish != nullptr && bitingfish->bBait)
// {
// 	//다른 물고기들이 타겟을 찾는다.
// 	 fish->fsm->UpdateSlowSwim();
// }