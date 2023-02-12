// Fill out your copyright notice in the Description page of Project Settings.


#include "FisherGameModeBase.h"

void AFisherGameModeBase::AddScore(int32 value)
{
	currScore += value;
	UE_LOG(LogTemp, Warning, TEXT("%d"), currScore);
}
