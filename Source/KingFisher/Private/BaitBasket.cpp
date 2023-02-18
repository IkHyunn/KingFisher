// Fill out your copyright notice in the Description page of Project Settings.


#include "BaitBasket.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>

ABaitBasket::ABaitBasket()
{
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(compBox);
	compBox->SetCollisionObjectType(ECC_GameTraceChannel3);

	baitBasketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bait Basket"));
	baitBasketMesh->SetupAttachment(RootComponent);

	baitMesh_1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_1"));
	baitMesh_1->SetupAttachment(baitBasketMesh);
	baitMesh_1->SetRelativeScale3D(FVector(4));

	baitMesh_2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_2"));
	baitMesh_2->SetupAttachment(baitBasketMesh);
	baitMesh_2->SetRelativeScale3D(FVector(4));

	baitMesh_3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_3"));
	baitMesh_3->SetupAttachment(baitBasketMesh);
	baitMesh_3->SetRelativeScale3D(FVector(4));

	baitMesh_4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_4"));
	baitMesh_4->SetupAttachment(baitBasketMesh);
	baitMesh_4->SetRelativeScale3D(FVector(4));

	baitMesh_5 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_5"));
	baitMesh_5->SetupAttachment(baitBasketMesh);
	baitMesh_5->SetRelativeScale3D(FVector(4));

	baitMesh_6 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_6"));
	baitMesh_6->SetupAttachment(baitBasketMesh);
	baitMesh_6->SetRelativeScale3D(FVector(4));

	baitMesh_7 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bait_7"));
	baitMesh_7->SetupAttachment(baitBasketMesh);
	baitMesh_7->SetRelativeScale3D(FVector(4));
}