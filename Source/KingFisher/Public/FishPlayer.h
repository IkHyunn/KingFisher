// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <HeadMountedDisplayTypes.h>
#include "FinishUIActor.h"
#include "FishPlayer.generated.h"

UCLASS()
class KINGFISHER_API AFishPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFishPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = "VR_Settings | ModuleSetting")
	TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere)
	class UFisherHandMesh* leftHand;

	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere)
	class UFisherHandMesh* rightHand;

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere)
	class UMoveComponent* compMove;

	UPROPERTY(EditAnywhere)
	class UGrabComponent* compGrab;

	UPROPERTY(EditAnywhere)
	TArray <class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere)
	TArray <class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* teleportTrace;

	TArray<FVector> baitThrowLocation;

	FVector linePos;
	FVector baitSpawnPos;
	FVector diagonal;
	FVector currBobberPos;

	FVector prevBaitLoc;
	FVector prevBaitForward;
	FVector throwDirection;

	FVector rotAxis;

	float angle;

	UPROPERTY(EditAnywhere)
	float throwPower = 500;

	int32 lineCount = 100;

	bool bthrowReady = false;
	bool bThrow = false;
	bool attachBait = false;
	bool bFishing = false;
	bool bReeling = false;
	bool bReleasing = false;
	bool timerOpen = false;
	bool bFinishOpen = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABait> baitFactory;

	UPROPERTY(EditAnywhere)
	class ABait* bait;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* menuPos;

	UPROPERTY(EditAnywhere)
	class UWidgetInteractionComponent* widgetPointer_Left;

	UPROPERTY(EditAnywhere)
	class UWidgetPointerComponent* compPointer;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* menuWidgetComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* timerWidgetComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFinishUIActor> finishUI;

	class AFisherGameModeBase* currGameMode;

	UPROPERTY(EditAnywhere)
	class USoundCue* sc_ThrowFishing;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* reelingSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* castSound;

	//물고기 사운드
	UPROPERTY(EditAnywhere)
	class USoundCue* catchfishSound;

	// fish_UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AfishWidgetActor> fish_Ui;
	float currTime = 0;

	bool bCatch = false; 

	float LeftH;
	float LeftV;
	float RightH;
	float RightV;

public:
	void OpenMenu();
	void OpenTimer();
	void ThrowReady();
	void ThrowRelease();
	void DrawBaitLine();
	void ThrowBait();
	void MoveBaitLine(const struct FInputActionValue& value);
	void BaitReleasing();
	void BaitReeling();

	void InputLeftHorizontal(float value);
	void InputLeftVertical(float value);
	void InputRightHorizontal(float value);
	void InputRightVertical(float value);


	void OpenFinishUI();




};
