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
	class ABait* target;

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
	float moveRange = 100.0f;

	// 미끼를 먹을 수 있는 반경
	float eatableRange = 5.0f;

	// 쫓아갈 수 있는 범위 
	float traceRange = 500.0f;

	// 먹는 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float EatDelayTime = 3.0f;

	// 피격 대기 시간
	float damageDelayTime = 3.0f;

	// ai 담을 변수
 	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	// 미끼 담을 변수
	UPROPERTY(EditAnywhere)
	class ABait* bait;

	// 감지 범위 
	float detectRange = 300.0f;

	// 현재 체력
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	float currHP = 0;

	// 최대 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	float maxHP = 3;

	bool bBite = false;


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

	 // 입질 시스템 **********
	void ControlRotation(float DeltaTime);

	 // 입질
	 float currentTime = 0.0f;
	 FVector startLoc;
	 FVector endLoc;
	 float direction = 1;
	 float speed = 0.02f;
	 FVector myLoc;
	 /*********************/
	 UPROPERTY(EditAnywhere)
	 TArray <class AFish*> fishArray;

	 UPROPERTY(EditAnywhere)
	 TArray <AActor*> collapsLocArray;

	 //***************


	 // 미끼 
	 UPROPERTY(EditDefaultsOnly)
	 TArray <AActor*> targetclass;

	 float distance;
	 float min;
	 float NewDistance;

	 //타겟과의 거리 구하기
	 /*void FindDistance();*/

	 // 추가 ****
	 bool bIsDead = false;

	 // 위젯 보여준다
	 void ShowWidget();

	 // Sweep방식으로 쫓기
	 UPROPERTY(EditAnywhere)
	 float Dest = 200.0f;

	 bool bHit;
	 

	


	 

	
	 
};
