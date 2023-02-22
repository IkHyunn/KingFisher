// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinishUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UFinishUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	class AFinishUIActor* finishUIActor;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* fishScore;

	int32 finalScore = 0;

	bool bIncreasingEnd = false;

	float currentTime = 0;

	class AFisherGameModeBase* currMode;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Restart;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Main;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Quit;

	UFUNCTION()
	void RestartButton();

	UFUNCTION()
	void MainButton();

	UFUNCTION()
	void QuitButton();
};
