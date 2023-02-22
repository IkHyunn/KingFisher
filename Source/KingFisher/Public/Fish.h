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

	// 콜리전 박스 생성
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* capsuleComp;

	// SkeletalMesh 랜덤으로 받기
	UPROPERTY (EditDefaultsOnly, Category = Mesh)
	TArray <class USkeletalMesh*> arrayMesh;

	// 애니메이션 랜덤 배열
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<TSubclassOf<class UFish_Anim>> FishAnimation;


	// 랜덤  SkeletalMesh 담을 함수
	void RandMesh ();

	// Mesh 담기
	int32 Rand = 0;

	// FishManger가져옴
	UPROPERTY(EditAnywhere)
	class AFishManager* fishmanager;

	//AI  관리하는 컴포넌트 (FSM)
	UPROPERTY(EditAnywhere)
	class UFish_FSM* fsm; 

	//Fish  활성  or  비활성
	void SetActive(bool bActive);


	// 컬러
	UPROPERTY (EditAnywhere)
	FLinearColor Fishcolor;

	// 밝기 세기
	UPROPERTY(EditAnywhere)
	float Power=1000;

	// 컬러 꺼질 때
	void ColorOff();

	
	// 물고기 머터리얼 
  	UPROPERTY (EditAnywhere, Category = Material)
  	TArray<class UMaterialInstance*> fishMat;

	//아웃라인 머터리얼 인스턴스
	UPROPERTY(EditAnywhere, Category = Material)
 	class UMaterialInstance* outlineMat;


	// 오버랩 되는 함수
	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 충돌 범위
	UPROPERTY(EditAnywhere)
	float grapDistance = 1.0f;


	//죽었을 떄 UI호출 *********************************(추가)*******
	/*DECLARE_DYNAMIC_DELEGATE(FOnFishDeath);*/

// 	UFUNCTION(BlueprintCallable)
// 	void OnFishDeath();
	


};
