// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "fishUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UfishUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_Release;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_Keep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* txt_experience;

	UPROPERTY(EditAnywhere)
	class AFisherGameModeBase* currMode;

	class AfishWidgetActor* fishUIActor; 

	bool bPopUpEnd = false; 

	float currTime = 0;

	int32 Score;

	UPROPERTY(EditAnywhere)
	class AFishPlayer* player; 

	//바인딩 함수
	UFUNCTION()
	void Release();

	UFUNCTION()
	void Keep();
	
}; 
