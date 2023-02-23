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
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "WidgetPointerComponent.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "MenuUIActor.h"
#include "TimerUI.h"
#include <Components/CapsuleComponent.h>
#include "FisherGameModeBase.h"
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>
#include "fishWidgetActor.h"



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

	widgetPointer_Left = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Left Widget Pointer"));
	widgetPointer_Left->SetupAttachment(leftController);

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

	menuPos = CreateDefaultSubobject<USceneComponent>(TEXT("FinishUI Spawn Pos"));
	menuPos ->SetupAttachment(RootComponent);
	menuPos->SetRelativeRotation(FRotator(0, 180, 0));

	bUseControllerRotationPitch = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	compMove=CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	compGrab=CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));
	compPointer=CreateDefaultSubobject<UWidgetPointerComponent>(TEXT("PointerComponent"));

	teleportTrace = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportTrace"));
	teleportTrace->SetupAttachment(rightHand);


	menuWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu Widget Component"));
	menuWidgetComp -> SetupAttachment(RootComponent);
	menuWidgetComp->SetVisibility(false);

	ConstructorHelpers::FClassFinder<UUserWidget>tempmenu(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/Widget/WG_GameMenuUI.WG_GameMenuUI_C'"));
	if (tempmenu.Succeeded())
	{
		menuWidgetComp->SetWidgetClass(tempmenu.Class);
		menuWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	}

	timerWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Timer Widget Component"));
	timerWidgetComp->SetupAttachment(leftController);
	timerWidgetComp->SetVisibility(false);

	ConstructorHelpers::FClassFinder<UUserWidget>temptimer(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/Widget/WG_Timer.WG_Timer_C'"));
	if (temptimer.Succeeded())
	{
		timerWidgetComp->SetWidgetClass(temptimer.Class);
		timerWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	}

	ConstructorHelpers::FObjectFinder<USoundCue>tempthrowsound(TEXT("/Script/Engine.SoundCue'/Game/Resources/Sound/sc_FOL_ThrowFishingRod.sc_FOL_ThrowFishingRod'"));
	if (tempthrowsound.Succeeded())
	{
		sc_ThrowFishing = tempthrowsound.Object;
	}

	reelingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Reeling Sound Component"));
	reelingSound->bAutoActivate = false;
	reelingSound->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USoundBase>tempcastingsound(TEXT("/Script/Engine.SoundWave'/Game/Resources/Sound/FOL_Casting.FOL_Casting'"));
	if (tempcastingsound.Succeeded())
	{
		castSound = tempcastingsound.Object;
	}
}

// Called when the game starts or when spawned
void AFishPlayer::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	APlayerController* playercontroller = GetWorld()->GetFirstPlayerController();

	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playercontroller->GetLocalPlayer());

	subSystem->AddMappingContext(myMapping, 0);
	
	currGameMode = Cast<AFisherGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AFishPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	if (bthrowReady && !bFishing)
// 	{
// 		DrawBaitLine();
// 	}

	if (bReeling)
	{
		FVector P0 = compGrab->fishingRod->bobberMesh->GetComponentLocation();
		FVector V = this->GetActorLocation()-P0;
		FVector P = P0+V*DeltaTime;

		if (compGrab->fishingRod->bBobberFloat)
		{
			compGrab->fishingRod->bobberMesh->SetWorldLocation(FVector(P.X, P.Y, P0.Z));
		}
		else
		{
			compGrab->fishingRod->bobberMesh->SetWorldLocation(FVector(P));
		}
	}

	if (bReleasing)
	{
		FVector P0 = compGrab->fishingRod->bobberMesh->GetComponentLocation();
		FVector V = -(this->GetActorLocation()-P0);
		FVector P = P0+V*DeltaTime;

		if (compGrab->fishingRod->bBobberFloat)
		{
			compGrab->fishingRod->bobberMesh->SetWorldLocation(FVector(P.X, P.Y, P0.Z));
		}
		else
		{
			compGrab->fishingRod->bobberMesh->SetWorldLocation(FVector(P));
		}
	}

	if (currGameMode != nullptr)
	{
		if (currGameMode->bCountEnd)
		{			
			if(!bFinishOpen)
			{
				OpenFinishUI();
			}
		}
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
	enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Started, this, &AFishPlayer::OpenMenu);
	enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Started, this, &AFishPlayer::OpenTimer);

	enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Started, this, &AFishPlayer::ThrowReady);
	enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &AFishPlayer::ThrowRelease);
	enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &AFishPlayer::MoveBaitLine);
	enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Started, this, &AFishPlayer::BaitReeling);
	enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Completed, this, &AFishPlayer::BaitReeling);
	enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &AFishPlayer::BaitReleasing);
	enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Completed, this, &AFishPlayer::BaitReleasing);

	compMove->SetupPlayerInputComponent(enhancedInputComponent);
	compGrab->SetupPlayerInputComponent(enhancedInputComponent);
	compPointer->SetupPlayerInputComponent(enhancedInputComponent);
}


void AFishPlayer::OpenMenu()
{
	if (currGameMode != nullptr)
	{
		if (menuWidgetComp->IsVisible() == false)
		{
			menuWidgetComp->SetVisibility(true);
		}
		else
		{
			menuWidgetComp->SetVisibility(false);
		}
	}
}

void AFishPlayer::OpenTimer()
{
	UTimerUI* timerUI = Cast<UTimerUI>(timerWidgetComp->GetWidget());

	if (currGameMode != nullptr)
	{
		if (!timerOpen)
		{
			UE_LOG(LogTemp, Warning, TEXT("Open"));
			timerUI->PlayAnimation(timerUI->PopupTimer, 0.0f, 1);
			timerOpen = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Close"));
			timerUI->PlayAnimationReverse(timerUI->PopupTimer, 1);
			timerOpen = false;
		}
	}
}

void AFishPlayer::ThrowReady()
{
	if (compGrab->fishingRod != nullptr)
	{
		bthrowReady = true;
		prevBaitLoc = rightController->GetComponentLocation();
	}
}

void AFishPlayer::ThrowRelease()
{
	if (compGrab->fishingRod != nullptr)
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
		//compGrab->fishingRod->bobberMesh->SetRelativeLocation(compGrab->fishingRod->throwPos->GetComponentLocation());
		//compGrab->fishingRod->bobberMesh->SetPhysicsLinearVelocity(diagonal * throwPower);
		throwDirection = rightController->GetComponentLocation() - prevBaitLoc;
		throwDirection.Normalize();
		compGrab->fishingRod->bobberMesh->SetRelativeLocation(rightController->GetComponentLocation());
		compGrab->fishingRod->bobberMesh->AddImpulse(throwDirection*500);
		UGameplayStatics::PlaySound2D(GetWorld(),sc_ThrowFishing);
		compGrab->fishingRod->pointConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 1000);
		compGrab->fishingRod->pointConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 1000);
		compGrab->fishingRod->pointConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 1000);
		bFishing = true;
	}
	else
	{
		compGrab->fishingRod->pointConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 30);
		compGrab->fishingRod->pointConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 30);
		compGrab->fishingRod->pointConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 30);
		compGrab->fishingRod->baitAttached = false;
		compGrab->fishingRod->bobberMesh->SetSimulatePhysics(true);
		compGrab->fishingRod->bBobberFloat = false;
		UGameplayStatics::PlaySound2D(GetWorld(), castSound);
		bFishing = false;

		if (bCatch)
		{
			FVector widgetLoc = GetActorLocation() + camera->GetForwardVector() * 100.0f;
			FRotator trans = FRotator(0, GetActorRotation().Yaw, 0);
			

			// fish_UI widget을 스폰 (위치, 방향)
			GetWorld()->SpawnActor<AfishWidgetActor>(fish_Ui, widgetLoc, trans);
			bCatch = false;

		}

	}
}

void AFishPlayer::MoveBaitLine(const struct FInputActionValue& value)
{
	if (compGrab->fishingRod != nullptr)
	{
		FVector2D axis = value.Get<FVector2D>();

		throwPower += axis.Y * 25;
		throwPower = FMath::Clamp(throwPower, 200, 1000);
	}
}

void AFishPlayer::BaitReleasing()
{
	if (!bReleasing && bFishing)
	{
		bReleasing = true;
		reelingSound->Play();
	}
	else
	{
		bReleasing = false;
		reelingSound->Stop();
	}
}

void AFishPlayer::BaitReeling()
{
	if (!bReeling && bFishing)
	{
		bReeling = true;
		reelingSound->Play();
	}
	else
	{
		bReeling = false;
		reelingSound->Stop();
	}
}

void AFishPlayer::OpenFinishUI()
{
	FVector openLoc = GetActorLocation()+GetActorForwardVector()*500;
	FRotator openRot = FRotator(0, GetActorRotation().Yaw+180, 0);

	GetWorld()->SpawnActor<AFinishUIActor>(finishUI, openLoc, openRot);
	bFinishOpen = true;
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