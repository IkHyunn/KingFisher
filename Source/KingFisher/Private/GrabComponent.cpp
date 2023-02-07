// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "FishPlayer.h"
#include "FisherHandAnim.h"
#include "FIsherHandMesh.h"
#include "TempFishingRod.h"
#include <CollisionQueryParams.h>
#include <Components/PrimitiveComponent.h>
#include <Components/SphereComponent.h>
#include "PickUpActor.h"
#include <Components/BoxComponent.h>

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AFishPlayer>(GetOwner());
	leftanim = Cast<UFisherHandAnim>(player->leftHand->GetAnimInstance());
	rightanim = Cast<UFisherHandAnim>(player->rightHand->GetAnimInstance());
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bGrab)
	{
		DrawGrabRange(player->rightHand);
	}
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &UGrabComponent::LeftGrabAction);
	PlayerInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &UGrabComponent::LeftGrabActionEnd);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &UGrabComponent::LeftIndexAction);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &UGrabComponent::LeftIndexAction);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Triggered, this, &UGrabComponent::LeftPointAction);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Completed, this, &UGrabComponent::LeftPointActionEnd);
	PlayerInputComponent->BindAction(leftInputs[3], ETriggerEvent::Triggered, this, &UGrabComponent::LeftThumbAction);
	PlayerInputComponent->BindAction(leftInputs[3], ETriggerEvent::Completed, this, &UGrabComponent::LeftThumbActionEnd);

	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UGrabComponent::RightGrabAction);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UGrabComponent::RightGrabActionEnd);
	PlayerInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &UGrabComponent::RightIndexAction);
	PlayerInputComponent->BindAction(rightInputs[1], ETriggerEvent::Completed, this, &UGrabComponent::RightIndexAction);
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Triggered, this, &UGrabComponent::RightPointAction);
	PlayerInputComponent->BindAction(rightInputs[2], ETriggerEvent::Completed, this, &UGrabComponent::RightPointActionEnd);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Triggered, this, &UGrabComponent::RightThumbAction);
	PlayerInputComponent->BindAction(rightInputs[3], ETriggerEvent::Completed, this, &UGrabComponent::RightThumbActionEnd);
}

void UGrabComponent::LeftGrabAction(const struct FInputActionValue& value)
{
	leftanim->poseGrasp=value.Get<float>();
	GrabObject(player->leftHand);
	bGrab = true;
}

void UGrabComponent::LeftGrabActionEnd(const struct FInputActionValue& value)
{
	leftanim->poseIndexCurl = value.Get<float>();
	ReleaseObject(player->leftHand);
	bGrab = false;
}

void UGrabComponent::LeftIndexAction(const struct FInputActionValue& value)
{
	leftanim->poseIndexCurl=value.Get<float>();
}

void UGrabComponent::LeftPointAction()
{
	leftanim->posePoint=1;
}

void UGrabComponent::LeftPointActionEnd()
{
	leftanim->posePoint=0;
}

void UGrabComponent::LeftThumbAction()
{
	leftanim->poseThumbUp=1;
}

void UGrabComponent::LeftThumbActionEnd()
{
	leftanim->poseThumbUp=0;
}

void UGrabComponent::RightGrabAction(const struct FInputActionValue& value)
{
	rightanim->poseGrasp=value.Get<float>();
	GrabObject(player->rightHand);
	bGrab = true;
}

void UGrabComponent::RightGrabActionEnd(const struct FInputActionValue& value)
{
	rightanim->poseGrasp = value.Get<float>();
	ReleaseObject(player->rightHand);
	bGrab = false;
}

void UGrabComponent::RightIndexAction(const struct FInputActionValue& value)
{
	rightanim->poseIndexCurl=value.Get<float>();
}

void UGrabComponent::RightPointAction()
{
	rightanim->posePoint=1;
}

void UGrabComponent::RightPointActionEnd()
{
	rightanim->posePoint=0;
}

void UGrabComponent::RightThumbAction()
{
	rightanim->poseThumbUp=1;
}

void UGrabComponent::RightThumbActionEnd()
{
	rightanim->poseThumbUp=0;
}

void UGrabComponent::GrabObject(USkeletalMeshComponent* hand)
{
	FVector startPos = hand->GetComponentLocation();
//	FVector endPos = startPos+hand->GetRightVector()*grabDistance;

	FHitResult hitinfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	bool bHit = GetWorld()->SweepSingleByProfile(hitinfo, startPos, startPos, FQuat::Identity, TEXT("PickUp"), FCollisionShape::MakeSphere(grabDistance), param);

	if (bHit && grabActor == nullptr)
	{
		// rodMesh = Cast<ATempFishingRod>(hitinfo.GetActor());
		grabActor = Cast<APickUpActor>(hitinfo.GetActor());

		if (IsValid(grabActor))
		{
			UBoxComponent* compBox = Cast<UBoxComponent>(grabActor->GetRootComponent());

			if (grabActor->GetRootComponent()->IsSimulatingPhysics() == true)
			{
				if (grabActor->GetName().Contains(TEXT("FishingRod")))
				{
					compBox->SetSimulatePhysics(false);
					grabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FishingRodPos"));
				}

				if (grabActor->GetName().Contains(TEXT("Bait")))
				{
					compBox->SetSimulatePhysics(false);

				}
			}
		}
	}
}

void UGrabComponent::ReleaseObject(USkeletalMeshComponent* hand)
{
	if (grabActor != nullptr)
	{
		UBoxComponent* compBox = Cast<UBoxComponent>(grabActor->GetRootComponent());

		if (compBox!=nullptr)
		{
			compBox->SetSimulatePhysics(true);
		}

		grabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		grabActor = nullptr;
	}
}

void UGrabComponent::DrawGrabRange(USkeletalMeshComponent* hand)
{
	if (grabActor == nullptr)
	{
		DrawDebugSphere(GetWorld(), hand->GetComponentLocation(), grabDistance, 30, FColor::Cyan, false, -1, 0, 0.5);
	}
}