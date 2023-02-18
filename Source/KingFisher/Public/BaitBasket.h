// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "BaitBasket.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API ABaitBasket : public APickUpActor
{
	GENERATED_BODY()
	
public:
	ABaitBasket();

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* baitBasketMesh;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_1;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_2;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_3;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_4;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_5;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_6;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh_7;
};
