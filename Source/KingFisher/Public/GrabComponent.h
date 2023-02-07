// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	class ATempFishingRod* rodMesh;

	UPROPERTY(EditAnywhere)
	class APickUpActor* pickUpActor;

	UPROPERTY(EditAnywhere)
	float grabDistance = 25;

	bool actorGrabbed = false;
	bool bGrab = false;
	bool leftGrab = false;

	UPROPERTY(EditAnywhere)
	class AActor* grabActor = nullptr;

public:
	void LeftGrabAction(const struct FInputActionValue& value);
	void LeftGrabActionEnd(const struct FInputActionValue& value);
	void LeftIndexAction(const struct FInputActionValue& value);
	void LeftPointAction();
	void LeftPointActionEnd();
	void LeftThumbAction();
	void LeftThumbActionEnd();

	void RightGrabAction(const struct FInputActionValue& value);
	void RightGrabActionEnd(const struct FInputActionValue& value);
	void RightIndexAction(const struct FInputActionValue& value);
	void RightPointAction();
	void RightPointActionEnd();
	void RightThumbAction();
	void RightThumbActionEnd();

	void GrabObject(USkeletalMeshComponent* hand);
	void ReleaseObject(USkeletalMeshComponent* hand);

	void DrawGrabRange(USkeletalMeshComponent* hand);
};
