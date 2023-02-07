// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Fish_FSM.generated.h"


// ��������
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

	// �������
	UPROPERTY (EditAnywhere)
	EFishState currState = EFishState::SlowSwim;

	// Ÿ��(�̳�)
	UPROPERTY (EditAnywhere, Category = FSM)
	class AFishBait* target;

	// �� (�����)
	UPROPERTY(EditAnywhere)
	class AFish* me;

	//aimInstance
	UPROPERTY(EditAnywhere)
	class UFish_Anim* anim;

	//animMontage
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EatMontage;




public:
	// ��� ���� ��ٸ��� �ð�
	UPROPERTY (EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2.0f;

	// ��� �ð�
	float currTime = 0;

	// ó����ġ
	FVector originPos;

	// ���� ��ġ
	FVector randPos;

	// �̵��ݰ�
	float moveRange = 2000.0f;

	// �̳����� �ٰ����� �ݰ�
	float feedRange = 100.0f;

	// �̳��� ���� �� �ִ� �ݰ�
	float eatableRange = 10.0f;

	// �Ѿư� �� �ִ� ���� 
	float traceRange = 1000.0f;

	// �Դ� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float EatDelayTime = 1.5f;

	// �ǰ� ��� �ð�
	float damageDelayTime = 2.0f;

	// ai ���� ����
 	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	//�׾��� �� �������� �ϴ� ����
	//bool bDieMove = false;


	// ���� ü��
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	float currHP;
	// �ִ� ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	float maxHP = 3;


	// ������� (���)
	void UpdateSlowSwim();
	// ���
	void UpdateSwim();
	// ���� ���
	void UpdateFastSwim();
	// �Ա�
	void UpdateEat();
	// �Ա� ���
	void UpdateEatDelay();
	// �̳��� �ɸ�
	void UpdateDamaged();
	// ����
	void UpdateDie();



	// ���ϻ���
	void UpdateReturnPos();

	// ���� ���� �� �� ���� ȣ��
	void ChangeState(EFishState State);

	// �̳��� ������ ��
	void ReceiveBait();

	// Ÿ���� �Ѿư� �� �ֳ�
	bool IsTargetTrace();

	// �ش� ��ġ ���� �� ���� ��ȯ�ϴ� �Լ�
	void MoveToPos (FVector pos);
	
	// Delay �Լ�
	 bool IsWaitComplete(float delayTime);

	 // �׺���̼� �Լ�
	 void Navigation();
};
