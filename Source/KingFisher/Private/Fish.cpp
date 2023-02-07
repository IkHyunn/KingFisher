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

	// Mesh  ��ġ ����
	//GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	// Mesh  ȸ�� ����
	//GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

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


	// SkeletalMesh_1
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_1(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/atlantic_cod_rig_exp20_SK.atlantic_cod_rig_exp20_SK'"));
	if (tempMesh_1.Succeeded())
	{
		arrayMesh.Add(tempMesh_1.Object);
	}


	// SkeletalMesh_2
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_2(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/barracuda_rig_exp20_SK.barracuda_rig_exp20_SK'"));
	if (tempMesh_2.Succeeded())
	{
		arrayMesh.Add(tempMesh_2.Object);
	}

	// SkeletalMesh_3
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_3(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/black_redeye_fish_rig_exp20_SK.black_redeye_fish_rig_exp20_SK'"));
	if (tempMesh_3.Succeeded())
	{
		arrayMesh.Add(tempMesh_3.Object);
	}

	// SkeletalMesh_4
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_4(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/black_spotted_grunt_rig_exp20_SK.black_spotted_grunt_rig_exp20_SK'"));
	if (tempMesh_4.Succeeded())
	{
		arrayMesh.Add(tempMesh_4.Object);
	}

	// SkeletalMesh_5
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_5(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/butterfly_fish_rig_exp20_SK.butterfly_fish_rig_exp20_SK'"));
	if (tempMesh_5.Succeeded())
	{
		arrayMesh.Add(tempMesh_5.Object);
	}

	// SkeletalMesh_6
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_6(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/carp_rig_exp20_SK.carp_rig_exp20_SK'"));
	if (tempMesh_6.Succeeded())
	{
		arrayMesh.Add(tempMesh_6.Object);
	}

	// SkeletalMesh_7
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_7(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/clownfish_rig_exp20_SK.clownfish_rig_exp20_SK'"));
	if (tempMesh_7.Succeeded())
	{
		arrayMesh.Add(tempMesh_7.Object);
	}

	// SkeletalMesh_8
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_8(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/discus_rig_exp20_SK.discus_rig_exp20_SK'"));
	if (tempMesh_8.Succeeded())
	{
		arrayMesh.Add(tempMesh_8.Object);
	}

	// SkeletalMesh_9
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_9(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/discus_v2_rig_exp20_SK.discus_v2_rig_exp20_SK'"));
	if (tempMesh_9.Succeeded())
	{
		arrayMesh.Add(tempMesh_9.Object);
	}

	// SkeletalMesh_10
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh_10(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/discus_v3_rig_exp20_SK.discus_v3_rig_exp20_SK'"));
	if (tempMesh_10.Succeeded())
	{
		arrayMesh.Add(tempMesh_10.Object);
	}
}


	void AFish::BeginPlay()
	{
		Super::BeginPlay();

		//�����̴� �������� ���� �ڵ����� ȸ���϶�� �ɼ�
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw =false;

		//���� SkeletalMesh ���� �Լ� ȣ��
		RandMesh();

		// ������ �Ž��� �����Ѵ�.
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


