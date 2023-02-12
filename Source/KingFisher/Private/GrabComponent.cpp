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
#include <MotionControllerComponent.h>
#include "Paddle.h"
#include "FishingRod.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>

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

	if (rightgrabActor == nullptr && bRightGrab == true)
	{
		DrawGrabRange(player->rightHand);
	}

	if (rightgrabActor == nullptr && bLeftGrab == true)
	{
		DrawGrabRange(player->leftHand);
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
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UGrabComponent::RighthandToShoot);
	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UGrabComponent::RighthandToShoot);
}

void UGrabComponent::LeftGrabAction(const struct FInputActionValue& value)
{
	leftanim->poseGrasp=value.Get<float>();
	LeftGrabObject(player->leftHand);
	bLeftGrab = true;
}

void UGrabComponent::LeftGrabActionEnd(const struct FInputActionValue& value)
{
	leftanim->poseIndexCurl = value.Get<float>();
	if (leftgrabActor != nullptr)
	{
		LeftReleaseObject(player->leftHand);
	}
	bLeftGrab = false;
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
	RightGrabObject(player->rightHand);
	bRightGrab = true;
}

void UGrabComponent::RightGrabActionEnd(const struct FInputActionValue& value)
{
	rightanim->poseGrasp = value.Get<float>();
	if (rightgrabActor != nullptr)
	{
		RightReleaseObject(player->rightHand, player->rightHand->GetComponentLocation());
	}

	bRightGrab = false;
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

void UGrabComponent::RightGrabObject(USkeletalMeshComponent* hand)
{
	FVector startPos = hand->GetComponentLocation();
//	FVector endPos = startPos+hand->GetRightVector()*grabDistance;

	FHitResult hitinfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	bool bHit = GetWorld()->SweepSingleByProfile(hitinfo, startPos, startPos, FQuat::Identity, TEXT("PickUp"), FCollisionShape::MakeSphere(grabDistance), param);

	if (bHit && rightgrabActor == nullptr)
	{
		// rodMesh = Cast<ATempFishingRod>(hitinfo.GetActor());
		rightgrabActor = Cast<APickUpActor>(hitinfo.GetActor());

		if (IsValid(rightgrabActor))
		{
			UBoxComponent* compBox = Cast<UBoxComponent>(rightgrabActor->GetRootComponent());
			if (rightgrabActor->GetRootComponent()->IsSimulatingPhysics() == true)
			{
				if (rightgrabActor->GetName().Contains(TEXT("FishingRod")))
				{
					fishingRod = Cast<AFishingRod>(hitinfo.GetActor());
					compBox->SetSimulatePhysics(false);
					rightgrabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FishingRodPos"));
					rightgrabActor->brightPickUp = true;
				}
				else if (rightgrabActor->GetName().Contains(TEXT("Paddle")))
				{
					compBox->SetSimulatePhysics(false);
					rightgrabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("PaddlePos_R"));
					rightgrabActor->brightPickUp = true;
				}

// 				if (rightgrabActor->GetName().Contains(TEXT("Basket")))
// 				{
// 					compBox->SetSimulatePhysics(false);
// 					rightgrabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BasketPos"));
// 					rightgrabActor->bactorPickUp == true;
// 				}
			}
		}
	}
}

void UGrabComponent::LeftGrabObject(USkeletalMeshComponent* hand)
{
	FVector startPos = hand->GetComponentLocation();
	//	FVector endPos = startPos+hand->GetRightVector()*grabDistance;

	FHitResult hitinfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	bool bHit = GetWorld()->SweepSingleByProfile(hitinfo, startPos, startPos, FQuat::Identity, TEXT("PickUp"), FCollisionShape::MakeSphere(grabDistance), param);

	if (bHit && leftgrabActor == nullptr)
	{
		leftgrabActor = Cast<APickUpActor>(hitinfo.GetActor());

		if (IsValid(leftgrabActor))
		{
			UBoxComponent* compBox = Cast<UBoxComponent>(leftgrabActor->GetRootComponent());

			if (leftgrabActor->GetRootComponent()->IsSimulatingPhysics() == true)
			{
				if (leftgrabActor->GetName().Contains(TEXT("Bait")))
				{
					compBox->SetSimulatePhysics(false);
					leftgrabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BaitPos"));
					leftgrabActor->bleftPickUp = true;
				}
			}
		}
	}
}

void UGrabComponent::RightReleaseObject(USkeletalMeshComponent* hand, FVector torque)
{
	if (rightgrabActor != nullptr && rightgrabActor->brightPickUp == true)
	{
		UBoxComponent* compBox = Cast<UBoxComponent>(rightgrabActor->GetRootComponent());
		if (compBox!=nullptr)
		{
			compBox->SetSimulatePhysics(true);
			rightgrabActor->brightPickUp = false;
		}

		// 구한 방향대로 힘을 가한다.
		throwLocation.Normalize();
		compBox->AddImpulse(throwLocation*throwPower);
		compBox->AddTorqueInDegrees(torque, NAME_None, true);

		rightgrabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	rightgrabActor = nullptr;
	fishingRod = nullptr;
}

void UGrabComponent::LeftReleaseObject(USkeletalMeshComponent* hand)
{
	if (leftgrabActor != nullptr && leftgrabActor->bleftPickUp == true)
	{
		UBoxComponent* compBox = Cast<UBoxComponent>(leftgrabActor->GetRootComponent());

		if (compBox != nullptr)
		{
			compBox->SetSimulatePhysics(true);
			leftgrabActor->bleftPickUp = false;
		}

		leftgrabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	leftgrabActor = nullptr;
}

void UGrabComponent::DrawGrabRange(USkeletalMeshComponent* hand)
{
	DrawDebugSphere(GetWorld(), hand->GetComponentLocation(), grabDistance, 30, FColor::Cyan, false, -1, 0, 0.5);
}

void UGrabComponent::RighthandToShoot()
{
	if (rightgrabActor != nullptr)
	{
		if (shootReady == false)
		{
			prevLocation = player->rightController->GetComponentLocation();
			prevForward = player->rightController->GetForwardVector();
		}
		else
		{
			// 던질 방향 저장
			throwLocation = player->rightController->GetComponentLocation() - prevLocation;

			// 회전 축
			FVector rotAxis = FVector::CrossProduct(prevForward, player->rightController->GetForwardVector());

			// 회전 양
			float angle = FMath::Acos(FVector::DotProduct(prevForward, player->rightController->GetForwardVector()));
			angle = FMath::RadiansToDegrees(angle);

			RightReleaseObject(player->rightHand, rotAxis * angle);
		}
		shootReady = !shootReady;
	}
}