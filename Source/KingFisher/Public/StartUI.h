// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGFISHER_API UStartUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	class AStartUIActor* startUIActor;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Start;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Quit;

public:
	UFUNCTION()
	void StartButton();

	UFUNCTION()
	void QuitButton();
};
