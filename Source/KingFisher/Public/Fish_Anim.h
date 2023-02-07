// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Fish_FSM.h"
#include "Fish_Anim.generated.h"


UCLASS()
class KINGFISHER_API UFish_Anim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;


public:

	UPROPERTY()
	class AFish* fish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	EFishState state;

	// �Դ� ���� ��� ����
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
 	bool bEatPlay = false;

	//�Դ� �ִϸ��̼� ����
	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndEat();




// 	UFUNCTION()
// 	void AnimNofity_FishRoaming();
// 
// 	UFUNCTION()
// 	void AnimNotify_DieEnd();

	
	
};
