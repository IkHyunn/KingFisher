

// Fill out your copyright notice in the Description page of Project Settings.
#include "Fish.h"
#include "Components/SphereComponent.h"
#include "Fish_Anim.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Fish_FSM.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Bait.h"
#include "Materials//MaterialInterface.h"
#include <Components/BoxComponent.h>

// Sets default values
AFish::AFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// 0. ���� ����
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MOUTH"));
	capsuleComp->SetCapsuleRadius(5);
	capsuleComp->SetupAttachment(RootComponent);
	capsuleComp->SetRelativeScale3D(FVector(15.0f, 2.0f, 0.3f));
	capsuleComp->SetRelativeLocation(FVector(0.0f,0.0f, 50.0f));



	// 1. �⺻ SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/atlantic_cod_rig_exp20_SK.atlantic_cod_rig_exp20_SK'"));
 	if (tempMesh.Succeeded())
 	{
 		GetMesh()-> SetSkeletalMesh(tempMesh.Object);
 	}

	//Mesh ũ�� ����
	GetMesh()->SetRelativeScale3D(FVector(3));


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


	// *****IH*****
	// Box Collision ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Grab Collision"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel3);

}


	void AFish::BeginPlay()
	{
		Super::BeginPlay();

		//�����̴� �������� ���� �ڵ����� ȸ���϶�� �ɼ�
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw =false;

		//���� SkeletalMesh ���� �Լ� ȣ��
		RandMesh();

		//�ٲ� �� ����
		GetMesh()->SetSkeletalMesh(nullptr);
		GetMesh()->SetAnimInstanceClass(nullptr);

		// ������ �Ž��� ����
		GetMesh()->SetSkeletalMesh(arrayMesh[Rand]);

		// ������ �ִϸ��̼� ����
		GetMesh()->SetAnimInstanceClass(FishAnimation[Rand]);

		// CapsuleComp�� overlap �Ǿ��� ��
		capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFish::Overlap);

		//Outline - ���͸��� �ν��Ͻ�
		outlineMat = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);

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

	//Outline ���� �ִ´�.
	GetMesh()->SetMaterial(0, outlineMat);

}

//Color off �Լ�
void AFish::ColorOff()
{
	// fishMat�� ���ش�.
	GetMesh()->SetMaterial(0, fishMat[Rand]);
	
}


void AFish::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != nullptr)
	{
		ABait* fishBait = Cast<ABait>(OtherActor);

		if (IsValid(fishBait))
		{
			fsm->UpdateEat();
		}	
	}	
}






