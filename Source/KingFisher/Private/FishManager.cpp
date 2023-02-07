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

	//1. ���� ���� �ð� ���ϱ�
	float createTime = FMath::RandRange(minTime, maxTime);
	//2. Timer Manager���� �˶����
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AFishManager::CreateFish, createTime);
	
	//���� ��ġ ���� �Ҵ� (ȣ��)
	FindSpawnPoints();
}


// Called every frame
void AFishManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFishManager::CreateFish()
{
	//���� ��ġ ���ϱ�
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	// ����� ���� �� ��ġ
	GetWorld()->SpawnActor<AFish>(fishFactory,
	spawnPoints[index]->GetActorLocation(), FRotator(0));

	// �ٽ� ���� �ð��� CreateFish �Լ��� ȣ��ǵ��� Ÿ�̸� ����
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle,
	this, &AFishManager::CreateFish, createTime);
}


void AFishManager::FindSpawnPoints()
{
	/*BP_FishSpawnPoint��� �̸����� ��ϵ� ���͵��� ��� ã��
	spawnPoint �迭�� �߰� */

	for (TActorIterator<AActor>It(GetWorld()); It; ++It)
		{
			AActor* spawn = *It;
			//ã�� ������ �̸��� �ش� ���ڿ��� �����ϰ� �ִٸ�
			if (spawn->GetName().Contains(TEXT("BP_FishSpawnPoint")))
			{
				//���� ��Ͽ� �߰�
				spawnPoints.Add(spawn);
			}
		}
}

// void AFishManager::FindSpawnPoints()
// {	
// 	//�˻����� ã�� ����� ������ �迭
// 	TArray<AActor*> allActors;
// 
// 	//���ϴ� Ÿ���� ���� ��� ã��
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
// 
// 
// }