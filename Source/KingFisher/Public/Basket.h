// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Basket.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API ABasket : public APickUpActor
{
	GENERATED_BODY()
	
public:
	ABasket();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* basketMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* handleMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* compFishBox;

	UPROPERTY(EditAnywhere)
	class USceneComponent* compScene;

	UPROPERTY(EditAnywhere)
	class USceneComponent* compRoot;

	UPROPERTY(EditAnywhere)
	class AFish* fish;

	class AFisherGameModeBase* gameMode;

	class AFishPlayer* player = nullptr;
	
	bool bGrabReady = false;

	bool bOpen = false;

	FVector handLoc;
	FVector lidLoc;
	float lidLocZ;

public:
	UFUNCTION()
	void OnGrab(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndGrab(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void GetFish(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void LostFish(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
