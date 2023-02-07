// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fish.generated.h"

UCLASS()
class KINGFISHER_API AFish : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	// SkeletalMesh 랜덤으로 받기
	UPROPERTY (EditDefaultsOnly, Category = Mesh)
	TArray <class USkeletalMesh*> arrayMesh;

	// 랜덤  SkeletalMesh 담을 함수
	void RandMesh ();

	// Mesh 담기
	int32 Rand = 0;

	// FishManger가져옴
	UPROPERTY(EditAnywhere)
	class AFishManager* fishmanager;

	//AI  관리하는 컴포넌트 (FSM)
	class UFish_FSM* fsm; 

	//Fish  활성  or  비활성
	void SetActive(bool bActive);

	// 죽었을 떄 호출되는 함수를 담는 변수****
	//FFishDieDelegate dieDelegate;

};
