// Fill out your copyright notice in the Description page of Project Settings.

#include "FishManager.h"
#include "Fish.h"
#include <EngineUtils.h>


// Sets default values
AFishManager::AFishManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFishManager::BeginPlay()
{
	Super::BeginPlay();

	//1. 랜덤 생성 시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	//2. Timer Manager한테 알람등록
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AFishManager::CreateFish, createTime);
	
	//스폰 위치 동적 할당 (호출)
	FindSpawnPoints();
}


// Called every frame
void AFishManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFishManager::CreateFish()
{
	//랜덤 위치 구하기
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	// 물고기 생성 및 배치
	GetWorld()->SpawnActor<AFish>(fishFactory,
	spawnPoints[index]->GetActorLocation(), FRotator(0));

	// 다시 랜덤 시간에 CreateFish 함수가 호출되도록 타이머 설정
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle,
	this, &AFishManager::CreateFish, createTime);
}


void AFishManager::FindSpawnPoints()
{
	/*BP_FishSpawnPoint라는 이름으로 등록된 액터들을 모두 찾아
	spawnPoint 배열에 추가 */

	for (TActorIterator<AActor>It(GetWorld()); It; ++It)
		{
			AActor* spawn = *It;
			//찾은 액터의 이름에 해당 문자열을 포함하고 있다면
			if (spawn->GetName().Contains(TEXT("BP_FishSpawnPoint")))
			{
				//스폰 목록에 추가
				spawnPoints.Add(spawn);
			}
		}
}

// void AFishManager::FindSpawnPoints()
// {	
// 	//검색으로 찾은 결과를 저장할 배열
// 	TArray<AActor*> allActors;
// 
// 	//원하는 타입의 액터 모두 찾기
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
// 
// 
// }