// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "Bait.h"
#include <../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h>
#include <PhysicsEngine/PhysicsConstraintComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SkeletalMeshComponent.h>
#include "FishPlayer.h"
#include "GrabComponent.h"
#include "FIsherHandMesh.h"
#include "EngineUtils.h"

AFishingRod::AFishingRod()
{
	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(compRoot);

	fishingRodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FishingRod Mesh"));
	fishingRodMesh->SetupAttachment(compRoot);
	fishingRodMesh->SetCollisionObjectType(ECC_GameTraceChannel3);
	fishingRodMesh->SetRelativeScale3D(FVector(2));
	fishingRodMesh->SetSimulatePhysics(true);

	pointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Point"));
	pointMesh->SetupAttachment(fishingRodMesh);
	pointMesh->SetRelativeLocation(FVector(84, -3.4, -1.5));
	pointMesh->SetRelativeRotation(FRotator(0, -90, 0));
	pointMesh->SetRelativeScale3D(FVector(0.005));

	bobberCable = CreateDefaultSubobject<UCableComponent>(TEXT("Bobber Cable"));
	bobberCable->SetupAttachment(fishingRodMesh);
	bobberCable->SetRelativeLocation(FVector(84, -3.25, -1.5));
	bobberCable->bAttachStart = true;
	bobberCable->bAttachEnd = true;
	bobberCable->CableWidth = 0.1;
	bobberCable->CableGravityScale = 0.01;
	bobberCable->SetAttachEndToComponent(bobberMesh);

	bobberMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bobber Mesh"));
	bobberMesh->SetupAttachment(fishingRodMesh);
	bobberMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	bobberMesh->SetSimulatePhysics(true);

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Bait Attach Collision"));
	compSphere->SetupAttachment(bobberMesh);

	pointConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Point Constraint"));
	pointConstraint->SetupAttachment(fishingRodMesh);
	pointConstraint->SetConstrainedComponents(pointMesh, NAME_None, bobberMesh, NAME_None);
	pointConstraint->SetRelativeLocation(FVector(84, -3.4, -1.5));
	pointConstraint->SetRelativeRotation(FRotator(180, 180, 0));

	throwPos = CreateDefaultSubobject<USceneComponent>(TEXT("Throw Pos"));
	throwPos->SetupAttachment(fishingRodMesh);
	throwPos->SetRelativeRotation(FRotator(0, -90, 0));
}

void AFishingRod::BeginPlay()
{
	Super::BeginPlay();

	compSphere->OnComponentBeginOverlap.AddDynamic(this, &AFishingRod::AttachBait);
}

void AFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBobberFloat)
	{
		FVector NewLocation = bobberMesh->GetComponentLocation();
		float deltaHeight = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		NewLocation.Z += deltaHeight * 10;
		runningTime += DeltaTime;

		bobberMesh->SetWorldLocation(NewLocation);
	}
}

void AFishingRod::AttachBait(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		ABait* bait = Cast<ABait>(OtherActor);

		if (OtherActor == bait)
		{
			UE_LOG(LogTemp, Warning, TEXT("True"));
			bait->AttachToComponent(bobberMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("baitSocket"));
			bait->compBox->SetSimulatePhysics(false);
			bait->bAttached = true;
			baitAttached = true;
		}
	}
}