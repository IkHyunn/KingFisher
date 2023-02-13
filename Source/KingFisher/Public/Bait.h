// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Bait.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API ABait : public APickUpActor
{
	GENERATED_BODY()
	
public:
	ABait();

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* BaitCollision;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh;

	bool bBaitReady = false;
};
