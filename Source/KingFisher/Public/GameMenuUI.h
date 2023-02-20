// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UGameMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	class AMenuUIActor* menuUIActor;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btn_Restart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btn_Settings;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btn_Resume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UOverlay* ovr_Settings;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* SettingsPopup;

	UPROPERTY(EditAnywhere)
		class AFishPlayer* player;

	bool bPopup = false;

public:
	UFUNCTION()
		void RestartButton();

	UFUNCTION()
		void SettingsButton();

	UFUNCTION()
		void ResumeButton();
};
