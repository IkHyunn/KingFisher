// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

APaddle::APaddle()
{
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(compBox);
	compBox->SetCollisionProfileName(TEXT("PickUp"));
	compBox->SetSimulatePhysics(true);

	paddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle"));
	paddleMesh->SetupAttachment(compBox);
	paddleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	compScene = CreateDefaultSubobject<USceneComponent>(TEXT("HandPos_R"));
}