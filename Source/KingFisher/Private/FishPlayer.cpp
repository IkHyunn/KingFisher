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

}

// Called to bind functionality to input
void AFishPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	compMove->SetupPlayerInputComponent(enhancedInputComponent);
	compGrab->SetupPlayerInputComponent(enhancedInputComponent);
}