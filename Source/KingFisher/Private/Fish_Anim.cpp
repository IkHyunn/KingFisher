// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish_Anim.h"
#include "Fish.h"

void UFish_Anim::NativeBeginPlay()
{
	//1. ���� �پ��ִ� Pawn �����´�.
	APawn* pawn = TryGetPawnOwner();
	//2.  pawn �� AFish �� ����ȯ
	fish = Cast<AFish>(pawn);
}

void UFish_Anim::OnEndEat()
{
	bEatPlay = false;
}




