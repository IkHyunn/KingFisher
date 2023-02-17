// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Bait.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API ABait : public APickUpActor
{
	GENERATED_BODY()
	
public:
	ABait();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* BaitCollision;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* baitMesh;


	//�������� ��ȣ�ۿ� 
	UPROPERTY(EditAnywhere)
	bool bBaitReady = true;

	UPROPERTY(EditAnywhere)
	class AFish* fish;

	UPROPERTY(EditAnywhere)
	bool bBait = false;

	UPROPERTY(EditAnywhere)
	float currenttime = 0;
};
