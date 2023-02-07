// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGFISHER_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

public:
	class AFishPlayer* player;
	bool bShowLine = false;
	TArray<FVector> lineLocation;

	UPROPERTY(EditAnywhere)
	float power = 300;

	UPROPERTY(EditAnywhere)
	TArray <class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere)
	TArray <class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, Category = "niagaraFX")
	TSubclassOf <class ATeleportRing> teleportSpot;

	class ATeleportRing* spawnedSpot;

public:
	void RotateAxis(const struct FInputActionValue& value);
	void ShowLine();
	void HideLine();
	void DrawMoveLine();
	void Teleport();
	void MoveTeleportPos(const struct FInputActionValue& value);
};
