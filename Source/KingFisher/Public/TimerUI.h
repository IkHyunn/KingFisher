// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UTimerUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Minutes;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Seconds;

	UPROPERTY(meta= (BindWidgetAnim), Transient)
	class UWidgetAnimation* TimeAlarm;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* PopupTimer;

	class AFishPlayer* player;

	class AFisherGameModeBase* currMode;
	
	float currentTime = 0;
	float delayTime = 1;

public:
	void TimeWarning();
};
