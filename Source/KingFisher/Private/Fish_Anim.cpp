// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish_Anim.h"
#include "Fish.h"

void UFish_Anim::NativeBeginPlay()
{
	//1. 내가 붙어있는 Pawn 가져온다.
	APawn* pawn = TryGetPawnOwner();
	//2.  pawn 이 AFish 로 형변환
	fish = Cast<AFish>(pawn);
}

void UFish_Anim::OnEndEat()
{
	bEatPlay = false;
}




