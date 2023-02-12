// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishManager.generated.h"

UCLASS()
class KINGFISHER_API AFishManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//  랜덤 시간 간격 최솟값
	UPROPERTY (EditAnywhere, Category = SpawnSettings)
	float minTime = 3;

	//  랜덤 시간 간격 최대값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float maxTime = 8;

	//스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TArray <class AActor*> spawnPoints;

	//AFish 타입의 블루프린트 할당받을 변수
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TSubclassOf <class AFish> fishFactory;


	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;

	int32 SpawnCount = 0;

	// 물고기 생성 함수
	void CreateFish();

	//스폰할 위치 동적 찾아 할당하기
	void FindSpawnPoints();

};
