// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

AFishingRod::AFishingRod()
{
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(compBox);
	compBox->SetCollisionProfileName(TEXT("PickUp"));
	compBox->SetSimulatePhysics(true);

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}