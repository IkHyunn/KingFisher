// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Fish_FSM.generated.h"


// 상태정의
UENUM(BlueprintType)
enum class EFishState : uint8
{
		SlowSwim,
		Swim,
		FastSwim,
		Eat,
		EatDelay,
		Damaged,
		Die,
		ReturnPos,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGFISHER_API UFish_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFish_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

public:

	// 현재상태
	UPROPERTY (EditAnywhere)
	EFishState currState = EFishState::SlowSwim;

	// 타겟(미끼)
	UPROPERTY (EditAnywhere, Category = FSM)
	class AFishBait* target;

	// 나 (물고기)
	UPROPERTY(EditAnywhere)
	class AFish* me;

	//aimInstance
	UPROPERTY(EditAnywhere)
	class UFish_Anim* anim;

	//animMontage
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EatMontage;




public:
	// 대기 상태 기다리는 시간
	UPROPERTY (EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2.0f;

	// 경과 시간
	float currTime = 0;

	// 처음위치
	FVector originPos;

	// 랜덤 위치
	FVector randPos;

	// 이동반경
	float moveRange = 2000.0f;

	// 미끼에게 다가가는 반경
	float feedRange = 100.0f;

	// 미끼를 먹을 수 있는 반경
	float eatableRange = 10.0f;

	// 쫓아갈 수 있는 범위 
	float traceRange = 1000.0f;

	// 먹는 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float EatDelayTime = 1.5f;

	// 피격 대기 시간
	float damageDelayTime = 2.0f;

	// ai 담을 변수
 	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	//죽었을 떄 내려가게 하는 변수
	//bool bDieMove = false;


	// 현재 체력
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	float currHP;
	// 최대 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	float maxHP = 3;


	// 느린헤엄 (대기)
	void UpdateSlowSwim();
	// 헤엄
	void UpdateSwim();
	// 빠른 헤엄
	void UpdateFastSwim();
	// 먹기
	void UpdateEat();
	// 먹기 대기
	void UpdateEatDelay();
	// 미끼에 걸린
	void UpdateDamaged();
	// 죽음
	void UpdateDie();



	// 리턴상태
	void UpdateReturnPos();

	// 상태 변경 시 한 번만 호출
	void ChangeState(EFishState State);

	// 미끼를 물었을 때
	void ReceiveBait();

	// 타겟을 쫓아갈 수 있나
	bool IsTargetTrace();

	// 해당 위치 도착 시 상태 전환하는 함수
	void MoveToPos (FVector pos);
	
	// Delay 함수
	 bool IsWaitComplete(float delayTime);

	 // 네비게이션 함수
	 void Navigation();
};
