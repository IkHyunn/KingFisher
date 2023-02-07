// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <HeadMountedDisplayTypes.h>
#include "FishPlayer.generated.h"

UCLASS()
class KINGFISHER_API AFishPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFishPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = "VR_Settings | ModuleSetting")
	TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere)
	class UFisherHandMesh* leftHand;

	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere)
	class UFisherHandMesh* rightHand;

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere)
	class UMoveComponent* compMove;

	UPROPERTY(EditAnywhere)
	class UGrabComponent* compGrab;

	UPROPERTY(EditAnywhere)
	TArray <class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere)
	TArray <class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* teleportTrace;
};
