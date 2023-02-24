// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "FishPlayer.h"
#include "FisherHandAnim.h"
#include "FIsherHandMesh.h"
#include <CollisionQueryParams.h>
#include <Components/PrimitiveComponent.h>
#include <Components/SphereComponent.h>
#include "PickUpActor.h"
#include <Components/BoxComponent.h>
#include <MotionControllerComponent.h>
#include "Paddle.h"
#include "FishingRod.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>
#include "Bait.h"
#include "Basket.h"
#include "Fish.h"
#include <Components/CapsuleComponent.h>

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

	// 	if (rightgrabActor == nullptr && bRightGrab == true)
	// 	{
	// 		DrawGrabRange(player->rightHand);
	// 	}
	// 
	// 	if (rightgrabActor == nullptr && bLeftGrab == true)
	// 	{
	// 		DrawGrabRange(player->leftHand);
	// 	}
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &UGrabComponent::LeftGrabAction);
	PlayerInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &UGrabComponent::LeftGrabActionEnd);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &UGrabComponent::LeftIndexAction);
	PlayerInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &UGrabComponent::LeftIndexAction);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Triggered, this, &UGrabComponent::LeftGrabFish);
	PlayerInputComponent->BindAction(leftInputs[2], ETriggerEvent::Completed, this, &UGrabComponent::LeftReleaseFish);
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
// 	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &UGrabComponent::RighthandToShoot);
// 	PlayerInputComponent->BindAction(rightInputs[4], ETriggerEvent::Completed, this, &UGrabComponent::RighthandToShoot);
}

void UGrabComponent::LeftGrabAction(const struct FInputActionValue& value)
{
	leftanim->poseGrasp = value.Get<float>();
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
	leftanim->poseIndexCurl = value.Get<float>();
}

void UGrabComponent::LeftGrabFish()
{
	leftanim->posePoint = 1;
//  	FVector startPos = player->leftHand->GetComponentLocation();
//  
//  	FHitResult hitinfo;
//  	FCollisionQueryParams param;
//  	param.AddIgnoredActor(player);
//  
//  	FCollisionObjectQueryParams objectParams;
//  	objectParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
//  
//  	bool bHit = GetWorld()->SweepSingleByObjectType(hitinfo, startPos, startPos, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(grabDistance), param);
//  
//  	if (bHit && fish == nullptr)
//  	{
//  		if (hitinfo.GetActor()->GetName().Contains(TEXT("Fish")))
//  		{
// 			fish = Cast<AFish>(hitinfo.GetActor());
// 			fish->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//  			fish->AttachToComponent(player->leftHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FishPos"));
// 			fish->GetCapsuleComponent()->SetSimulatePhysics(false);
//  		}
//  	}
}

void UGrabComponent::LeftReleaseFish()
{
	leftanim->posePoint = 0;

 	if (fish != nullptr)
 	{
 		fish->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
 		fish->GetCapsuleComponent()->SetSimulatePhysics(true);
 		fish->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
 		fish = nullptr;
 	}
}

void UGrabComponent::LeftThumbAction()
{
	leftanim->poseThumbUp = 1;
}

void UGrabComponent::LeftThumbActionEnd()
{
	leftanim->poseThumbUp = 0;
}


void UGrabComponent::RightGrabAction(const struct FInputActionValue& value)
{
	rightanim->poseGrasp = value.Get<float>();
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
	rightanim->poseIndexCurl = value.Get<float>();
}

void UGrabComponent::RightPointAction()
{
	rightanim->posePoint = 1;
}

void UGrabComponent::RightPointActionEnd()
{
	rightanim->posePoint = 0;
}

void UGrabComponent::RightThumbAction()
{
	rightanim->poseThumbUp = 1;
}

void UGrabComponent::RightThumbActionEnd()
{
	rightanim->poseThumbUp = 0;
}

void UGrabComponent::RightGrabObject(USkeletalMeshComponent* hand)
{
	FVector startPos = hand->GetComponentLocation();
//	FVector endPos = startPos+hand->GetRightVector()*grabDistance;

	FHitResult hitinfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);

	bool bHit = GetWorld()->SweepSingleByObjectType(hitinfo, startPos, startPos, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(grabDistance), param);

	if (bHit && rightgrabActor == nullptr)
	{
		rightgrabActor = Cast<APickUpActor>(hitinfo.GetActor());

		if (IsValid(rightgrabActor))
		{
			UBoxComponent* compBox = Cast<UBoxComponent>(rightgrabActor->GetRootComponent());
//			if (rightgrabActor->GetRootComponent()->IsSimulatingPhysics() == true)
//			{
				if (rightgrabActor->GetName().Contains(TEXT("FishingRod")))
				{
					fishingRod = Cast<AFishingRod>(hitinfo.GetActor());
					fishingRod->fishingRodMesh->SetSimulatePhysics(false);
					fishingRod->SetActorLocation(fishingRod->fishingRodMesh->GetComponentLocation());
					fishingRod->SetActorRotation(fishingRod->fishingRodMesh->GetComponentRotation());
					fishingRod->fishingRodMesh->AttachToComponent(fishingRod->compRoot,FAttachmentTransformRules::KeepWorldTransform);
					fishingRod->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*FString(TEXT("FishingRodPos"))));
					fishingRod->SetActorRelativeLocation(fishingRod->gripOffset);
					fishingRod->SetActorRelativeRotation(FRotator(0));
					fishingRod->brightPickUp = true;
				}
				else if (rightgrabActor->GetName().Contains(TEXT("Paddle")))
				{
					compBox->SetSimulatePhysics(false);
					rightgrabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("PaddlePos_R"));
					rightgrabActor->brightPickUp = true;
				}

//			}
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

	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);

	bool bHit = GetWorld()->SweepSingleByObjectType(hitinfo, startPos, startPos, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(grabDistance), param);

	if (bHit && leftgrabActor == nullptr)
	{
		leftgrabActor = Cast<APickUpActor>(hitinfo.GetActor());

		if (IsValid(leftgrabActor))
		{
			UBoxComponent* compBox = Cast<UBoxComponent>(leftgrabActor->GetRootComponent());

		//	if (leftgrabActor->GetRootComponent()->IsSimulatingPhysics() == true)
		//	{
				if (leftgrabActor->GetName().Contains(TEXT("Bait")))
				{
					bait = Cast<ABait>(hitinfo.GetActor());
					bait->compBox->SetSimulatePhysics(false);
					leftgrabActor->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BaitPos"));
					leftgrabActor->bleftPickUp = true;
				}
				else if (leftgrabActor->GetName().Contains(TEXT("Feed")))
				{
					FActorSpawnParameters spawnParameters;
					spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					bait = GetWorld()->SpawnActor<ABait>(baitFactory, hand->GetComponentLocation(), hand->GetComponentRotation(),spawnParameters);
					bait->AttachToComponent(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BaitPos"));
					bait->compBox->SetSimulatePhysics(false);
					leftgrabActor->bleftPickUp = true;
				}
				else if (leftgrabActor->GetName().Contains(TEXT("Basket")))
				{
					basket = Cast<ABasket>(hitinfo.GetActor());
					basket->player = player;
					basket->bOpen = true;
					basket->handLoc = player->leftHand->GetComponentLocation();
					leftgrabActor->bleftPickUp = true;
				}
		//	}
		}
	}
}

void UGrabComponent::RightReleaseObject(USkeletalMeshComponent* hand, FVector torque)
{
	if (rightgrabActor != nullptr && rightgrabActor->brightPickUp == true)
	{
		UBoxComponent* compBox = Cast<UBoxComponent>(rightgrabActor->GetRootComponent());
		if (compBox != nullptr)
		{
			//fishingRod->baitMesh->SetSimulatePhysics(false);
			//rightgrabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			//ABait* curBait = Cast<ABait>( fishingRod->baitActor->GetChildActor());
// 			fishingRod->baitchild->compBox->SetPhysicsLinearVelocity(FVector::ZeroVector);
// 			fishingRod->baitchild->compBox->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
// 			fishingRod->pointConstraint->SetRelativeLocation(FVector(221, 0, 0));
// 			fishingRod->pointConstraint->SetRelativeRotation(FRotator(180, 180, 0));


			//fishingRod->baitMesh->SetSimulatePhysics(true);

// 			compBox->SetPhysicsLinearVelocity(FVector::ZeroVector);
// 			compBox->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

			rightgrabActor->brightPickUp = false;
		}

		// 구한 방향대로 힘을 가한다.
		//throwLocation.Normalize();
		//compBox->AddImpulse(throwLocation * throwPower);
		//compBox->AddTorqueInDegrees(torque, NAME_None, true);

		if (fishingRod != nullptr)
		{
			fishingRod->fishingRodMesh->SetSimulatePhysics(true);
			rightgrabActor->brightPickUp = false;
		}
	}
	rightgrabActor = nullptr;
//	fishingRod = nullptr;
}

void UGrabComponent::LeftReleaseObject(USkeletalMeshComponent* hand)
{
	if (leftgrabActor != nullptr && leftgrabActor->bleftPickUp == true)
	{
//		UBoxComponent* compBox = Cast<UBoxComponent>(leftgrabActor->GetRootComponent());

// 		if (compBox != nullptr)
// 		{
// 			compBox->SetSimulatePhysics(true);
// 			leftgrabActor->bleftPickUp = false;
// 			leftgrabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		}

		if (bait != nullptr && !bait->bAttached)
		{
			bait->compBox->SetSimulatePhysics(true);
			leftgrabActor->bleftPickUp = false;
			leftgrabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		if (basket != nullptr)
		{
			basket->bOpen = false;
			leftgrabActor->brightPickUp = false;
		}
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

