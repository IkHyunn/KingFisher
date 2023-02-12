// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Paddle.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API APaddle : public APickUpActor
{
	GENERATED_BODY()
	
public:
	APaddle();

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* paddleMesh;

	UPROPERTY(EditAnywhere)
	class USceneComponent* compScene;
};
