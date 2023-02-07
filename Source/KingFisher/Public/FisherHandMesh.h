// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "FisherHandMesh.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UFisherHandMesh : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UFisherHandMesh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool mirror = false;
};
