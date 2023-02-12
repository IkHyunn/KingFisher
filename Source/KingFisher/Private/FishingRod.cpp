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

AFishingRod::AFishingRod()
{
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(compBox);
	compBox->SetCollisionProfileName(TEXT("PickUp"));
	compBox->SetSimulatePhysics(true);
	compBox->SetBoxExtent(FVector(50, 3, 3));

	fishingRodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	fishingRodMesh->SetupAttachment(compBox);
	fishingRodMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	fishingRodMesh->SetRelativeScale3D(FVector(2));
	fishingRodMesh->SetRelativeLocation(FVector(53, 7, 3));

	pointMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Point"));
	pointMesh->SetupAttachment(fishingRodMesh);
	pointMesh->SetRelativeLocation(FVector(84, -3.4, -1.5));
	pointMesh->SetRelativeRotation(FRotator(0, -90, 0));
	pointMesh->SetRelativeScale3D(FVector(0.005));

	rodCable_1 = CreateDefaultSubobject<UCableComponent>(TEXT("RodCable_1"));
	rodCable_1->SetupAttachment(fishingRodMesh);
	rodCable_1->SetRelativeLocation(FVector(-32.5, -3, -1.5));
	rodCable_1->bAttachStart = true;
	rodCable_1->bAttachEnd = true;
	rodCable_1->CableWidth = 0.1;
	rodCable_1->CableGravityScale = 0;
	
	rodCable_2 = CreateDefaultSubobject<UCableComponent>(TEXT("RodCable_2"));
	rodCable_2->SetupAttachment(fishingRodMesh);
	rodCable_2->SetRelativeLocation(FVector(84, -3.25, -1.5));
	rodCable_2->bAttachStart = true;
	rodCable_2->bAttachEnd = true;
	rodCable_2->CableWidth = 0.1;
	rodCable_2->CableGravityScale = 0.01;
	rodCable_2->SetAttachEndToComponent(baitMesh);

	baitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bait Mesh"));
	baitMesh->SetupAttachment(compBox);
	baitMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	compSphere->SetupAttachment(baitMesh);

	pointConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Point Constraint"));
	pointConstraint->SetupAttachment(compBox);
	pointConstraint->SetConstrainedComponents(pointMesh, NAME_None, baitMesh, NAME_None);

	baitActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Bait Actor"));
	baitActor->SetupAttachment(baitMesh);

	ConstructorHelpers::FClassFinder<ABait>tempbait(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_Bait.BP_Bait_C'"));
	if (tempbait.Succeeded())
	{
		baitActor->SetChildActorClass(tempbait.Class);
	}

	baitConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Bait Constraint"));
	baitConstraint->SetupAttachment(compBox);

	throwPos = CreateDefaultSubobject<USceneComponent>(TEXT("Throw Pos"));
	throwPos -> SetupAttachment(compBox);
	throwPos -> SetRelativeRotation(FRotator(0, -90, 0));
}

void AFishingRod::BeginPlay()
{
	Super::BeginPlay();
	
	compSphere->OnComponentBeginOverlap.AddDynamic(this, &AFishingRod::AttachBait);
	
	baitchild = Cast<ABait>(baitActor->GetChildActor());
	baitConstraint->SetConstrainedComponents(baitMesh, NAME_None, baitchild->compBox, NAME_None);
	baitchild->compBox->SetVisibility(false);
	baitchild->baitMesh->SetVisibility(false);
}

void AFishingRod::AttachBait(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (baitchild->compBox->IsVisible()==false && baitchild->baitMesh->IsVisible()==false)
	{
		ABait* bait = Cast<ABait>(OtherActor);

		if (OtherActor == bait)
		{
			baitchild->compBox->SetVisibility(true);
			baitchild->baitMesh->SetVisibility(true);
			bait->Destroy();
			baitAttached = true;
		}
	}
}