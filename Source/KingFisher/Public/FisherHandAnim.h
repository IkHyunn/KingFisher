// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FisherHandAnim.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UFisherHandAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float poseGrasp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float poseIndexCurl = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float posePoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float poseThumbUp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	bool Mirror = false;
};
