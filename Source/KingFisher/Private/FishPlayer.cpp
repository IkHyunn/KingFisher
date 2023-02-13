// Fill out your copyright notice in the Description page of Project Settings.


#include "FishPlayer.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "MoveComponent.h"
#include "GrabComponent.h"
#include "EnhancedInputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "FisherHandAnim.h"
#include "FIsherHandMesh.h"
#include "Bait.h"
#include <Components/BoxComponent.h>
#include "FishingRod.h"
#include <PhysicsEngine/PhysicsConstraintComponent.h>


// Sets default values
AFishPlayer::AFishPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(RootComponent);
	
	headMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	headMesh->SetupAttachment(camera);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> temphead(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/Devices/Generic/GenericHMD.GenericHMD'"));
	if (temphead.Succeeded())
	{
		headMesh->SetStaticMesh(temphead.Object);
	}

	leftController=CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	leftController->SetupAttachment(RootComponent);
	leftController->MotionSource = "Left";
	
	leftHand=CreateDefaultSubobject<UFisherHandMesh>(TEXT("LeftHand"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetRelativeRotation(FRotator(-25, 180, 90));
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> templeft(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (templeft.Succeeded())
	{
		leftHand->SetSkeletalMesh(templeft.Object);
		leftHand->mirror=true;
	}

	rightController=CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";
	
	rightHand=CreateDefaultSubobject<UFisherHandMesh>(TEXT("RightHand"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetRelativeRotation(FRotator(25, 0, 90));
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	baitPos = CreateDefaultSubobject<USceneComponent>(TEXT("bait Spawn Pos"));
	baitPos ->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	compMove=CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	compGrab=CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));

	teleportTrace = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportTrace"));
	teleportTrace->SetupAttachment(rightHand);
}

// Called when the game starts or when spawned
void AFishPlayer::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	APlayerController* playercontroller = GetWorld()->GetFirstPlayerController();

	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playercontroller->GetLocalPlayer());

	subSystem->AddMappingContext(myMapping, 0);
	
}

// Called every frame
void AFishPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bthrowReady && bFishing == false && compGrab->fishingRod->baitAttached == true)
	{
		DrawBaitLine();
	}

// 	FVector LeftP0 = leftController->GetComponentLocation();
// 	FVector LeftHor = leftController->GetRightVector()*LeftH;
// 	FVector LeftVer = leftController->GetUpVector()*LeftV;
// 
// 	FVector LeftP = LeftP0 + (LeftHor+LeftVer)*500*DeltaTime;
// 	leftController->SetRelativeLocation(LeftP);
// 
// 	FVector RightP0 = rightController->GetComponentLocation();
// 	FVector RightHor = rightController->GetRightVector()*RightH;
// 	FVector RightVer = rightController->GetUpVector()*RightV;
// 
// 	FVector RightP = RightP0 + (RightHor+RightVer)*500*DeltaTime;
// 	rightController->SetRelativeLocation(RightP);
}

// Called to bind functionality to input
void AFishPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

// 	PlayerInputComponent->BindAxis(TEXT("lefthandhorizontal"), this, &AFishPlayer::InputLeftHorizontal);
// 	PlayerInputComponent->BindAxis(TEXT("lefthandvertical"), this, &AFishPlayer::InputLeftVertical);
// 	PlayerInputComponent->BindAxis(TEXT("righthandhorizontal"), this, &AFishPlayer::InputRightHorizontal);
// 	PlayerInputComponent->BindAxis(TEXT("righthandvertical"), this, &AFishPlayer::InputRightVertical);

	enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Started, this, &AFishPlayer::ThrowReady);
	enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &AFishPlayer::ThrowRelease);
	enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &AFishPlayer::MoveBaitLine);

	compMove->SetupPlayerInputComponent(enhancedInputComponent);
	compGrab->SetupPlayerInputComponent(enhancedInputComponent);
}

void AFishPlayer::ThrowReady()
{
	if (compGrab->fishingRod != nullptr && compGrab->fishingRod->baitAttached == true)
	{
		bthrowReady = true;
	}
}

void AFishPlayer::ThrowRelease()
{
	if (compGrab->fishingRod != nullptr && compGrab->fishingRod->baitAttached == true)
	{
		bthrowReady = false;
		ThrowBait();
	}
}

void AFishPlayer::DrawBaitLine()
{
	float delayTime = 0.1;
	lineCount = 100;

	FVector handForward = compGrab->fishingRod->throwPos->GetForwardVector();
	FVector handUp = compGrab->fishingRod->throwPos->GetRightVector();

// 	FVector handForward = FRotationMatrix(rightHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
// 	FVector handUp = FRotationMatrix(rightHand->GetComponentRotation()).GetUnitAxis(EAxis::X);
//	FVector handForward = rightHand->GetRightVector();
//	FVector handUp = rightHand->GetForwardVector();

	diagonal = handForward + handUp;
	baitThrowLocation.Empty();

	for (int32 i = 0; i < lineCount; i++)
	{
		float timeTaken = delayTime * i;
		linePos = compGrab->fishingRod->throwPos->GetComponentLocation() + diagonal * throwPower * timeTaken;
		linePos.Z += 0.5*GetWorld()->GetDefaultGravityZ()*timeTaken*timeTaken;
		baitThrowLocation.Add(linePos);
		FHitResult hitinfo;
		if (i > 0)
		{
			if (GetWorld()->LineTraceSingleByChannel(hitinfo, baitThrowLocation[i - 1], baitThrowLocation[i], ECC_Visibility))
			{
				baitThrowLocation.Add(hitinfo.ImpactPoint);
				break;
			}
		}
	}

	for (int32 i = 0; i < baitThrowLocation.Num()-1; i++)
	{
		DrawDebugLine(GetWorld(),baitThrowLocation[i], baitThrowLocation[i+1], FColor::Red, false, -1, 0, 1.5);
	}
}

void AFishPlayer::ThrowBait()
{
	if (bFishing == false)
	{
		compGrab->fishingRod->baitMesh->SetRelativeLocation(compGrab->fishingRod->throwPos->GetComponentLocation());
		compGrab->fishingRod->baitMesh->SetPhysicsLinearVelocity(diagonal * throwPower);
		compGrab->fishingRod->pointConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 1000);
		compGrab->fishingRod->pointConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 1000);
		compGrab->fishingRod->pointConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 1000);
		bFishing = true;
	}
	else
	{
		compGrab->fishingRod->pointConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 200);
		compGrab->fishingRod->pointConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 200);
		compGrab->fishingRod->pointConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 200);
		compGrab->fishingRod->baitchild->baitMesh->SetVisibility(false);
		compGrab->fishingRod->baitAttached = false;
		bFishing = false;
	}
}

void AFishPlayer::MoveBaitLine(const struct FInputActionValue& value)
{
	if (compGrab->fishingRod != nullptr && compGrab->fishingRod->baitAttached == true)
	{
		float val = value.Get<float>();

		throwPower += val * 25;
		throwPower = FMath::Clamp(throwPower, 200, 1000);
	}
}

// void AFishPlayer::InputLeftHorizontal(float value)
// {
// 	LeftH = value;
// }

//void AFishPlayer::InputLeftVertical(float value)
// {
// 	LeftV = value;
// }

//void AFishPlayer::InputRightHorizontal(float value)
// {
// 	RightH = value;
// }

// void AFishPlayer::InputRightVertical(float value)
// {
//	RightV = value;
//}