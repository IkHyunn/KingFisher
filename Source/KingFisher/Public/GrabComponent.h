// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Components/SkeletalMeshComponent.h>
#include "GrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGFISHER_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

public:
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TArray <class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TArray <class UInputAction*> rightInputs;

	class AFishPlayer* player;
	class UFisherHandAnim* leftanim;
	class UFisherHandAnim* rightanim;

	UPROPERTY(EditAnywhere)
	class APickUpActor* pickUpActor;

	UPROPERTY(EditAnywhere)
	float grabDistance = 25;

	UPROPERTY(EditAnywhere)
	float throwPower = 1000;

	bool actorGrabbed = false;
	bool bHandGrab = false;
	bool bRightGrab = false;
	bool bLeftGrab = false;
	bool shootReady = false;

	class APickUpActor* rightgrabActor = nullptr;
	class APickUpActor* leftgrabActor = nullptr;

	class AActor* grabActor = nullptr;
	class AFishingRod* fishingRod = nullptr;
	class ABasket* basket = nullptr;
	class ABait* bait;
	class AFish* fish;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABait> baitFactory;

	FVector prevLocation;
	FVector throwLocation;
	FVector prevForward;

public:
	void LeftGrabAction(const struct FInputActionValue& value);
	void LeftGrabActionEnd(const struct FInputActionValue& value);
	void LeftIndexAction(const struct FInputActionValue& value);
	void LeftGrabFish();
	void LeftReleaseFish();
	void LeftThumbAction();
	void LeftThumbActionEnd();

	void RightGrabAction(const struct FInputActionValue& value);
	void RightGrabActionEnd(const struct FInputActionValue& value);
	void RightIndexAction(const struct FInputActionValue& value);
	void RightPointAction();
	void RightPointActionEnd();
	void RightThumbAction();
	void RightThumbActionEnd();

	void RightGrabObject(USkeletalMeshComponent* hand);
	void RightReleaseObject(USkeletalMeshComponent* hand, FVector torque);
	void LeftGrabObject(USkeletalMeshComponent* hand);
	void LeftReleaseObject(USkeletalMeshComponent* hand);

	void DrawGrabRange(USkeletalMeshComponent* hand);
	void RighthandToShoot();

	
};
