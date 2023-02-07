// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "FishingRod.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API AFishingRod : public APickUpActor
{
	GENERATED_BODY()
	
public:
	AFishingRod();

public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* compMesh;
};
