// Fill out your copyright notice in the Description page of Project Settings.


#include "FisherHandMesh.h"
#include "../Public/FisherHandAnim.h"

UFisherHandMesh::UFisherHandMesh()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> temphand(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (temphand.Succeeded())
	{
		SetSkeletalMesh(temphand.Object);
	}

	ConstructorHelpers::FClassFinder<UFisherHandAnim> tempanim(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/MannequinsXR/Meshes/ABP_MannequinsXR.ABP_MannequinsXR_C'"));
	if (tempanim.Succeeded())
	{
		SetAnimInstanceClass(tempanim.Class);
	}
}