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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* fishingRodMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* compSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UChildActorComponent* baitActor;

	UPROPERTY(EditAnywhere)
		class UCableComponent* rodCable_1;

	UPROPERTY(EditAnywhere)
		class UCableComponent* rodCable_2;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* baitMesh;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* pointMesh;

	UPROPERTY(EditAnywhere)
		class UPhysicsConstraintComponent* pointConstraint;

	UPROPERTY(EditAnywhere)
		class UPhysicsConstraintComponent* baitConstraint;

	UPROPERTY(EditAnywhere)
		class AFishPlayer* player;

	UPROPERTY(EditAnywhere)
		class USceneComponent* throwPos;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABait> baitFactory;

	class ABait* baitchild;

	bool baitAttached = false;

public:
	UFUNCTION()
	void AttachBait(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
