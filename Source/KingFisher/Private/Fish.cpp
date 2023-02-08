// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish.h"
#include "Fish_Anim.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Fish_FSM.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
AFish::AFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 1. �⺻ SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/arapaima_rig_exp20_SK.arapaima_rig_exp20_SK'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()-> SetSkeletalMesh(tempMesh.Object);
	}

	//Mesh ũ�� ����
	GetMesh()->SetWorldScale3D(FVector(10));



	//2. �ִϸ��̼� Ŭ���� ����
	ConstructorHelpers::FClassFinder<UFish_Anim> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrints/Fish_Anim/ABP_Fish.ABP_Fish_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	//3. FSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<UFish_FSM>(TEXT("FSM"));
	
	//4. LineTrace �� ������ �ǰ� ����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	//5.  AutoPossess AI ���� 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


}


	void AFish::BeginPlay()
	{
		Super::BeginPlay();

		//�����̴� �������� ���� �ڵ����� ȸ���϶�� �ɼ�
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw =false;

		//���� SkeletalMesh ���� �Լ� ȣ��
		RandMesh();

		// ������ �Ž��� �����Ѵ�.*****
		GetMesh()->SetSkeletalMesh(arrayMesh[Rand]);
		
	}


	void AFish::SetActive(bool bActive)
	{
		if (bActive)
		{
			GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::QueryAndPhysics));
			// ������ġ �缳��
			fsm->originPos = GetActorLocation();
		}
		
		else
		{
			// �浹 ��Ȱ��ȭ
			GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
			// źâ�� �� �ٽ� �־��
			//dieDelegate.ExecuteIfBound(this);    //this, �� �� �ڽ��� �־���.
		}

		//  �Ž��� Ȱ��/ ��Ȱ��ȭ
		GetMesh()->SetActive(bActive);
		// �Ž� ���̰�/ �Ⱥ��̰�
		GetMesh()->SetVisibility(bActive);
		// ĳ���� �����Ʈ Ȱ��/ ��Ȱ��
		GetCharacterMovement()->SetActive(bActive);
		// fsm  Ȱ��/ ��Ȱ��
		fsm->SetActive(bActive);

}


// Called every frame
void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFish::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ����  SkeletalMesh ���� �Լ�
void AFish::RandMesh()
{
	//Skeletal Mesh ���� ���
	Rand = FMath::RandRange(0, 9);

}


