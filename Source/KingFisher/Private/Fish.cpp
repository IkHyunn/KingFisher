

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

// Sets default values
AFish::AFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// 0. 소켓 생성
	
		capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MOUTH"));
		capsuleComp->SetCapsuleRadius(5);
		capsuleComp->SetupAttachment(RootComponent);
		//capsuleComp->SetCollisionProfileName (TEXT("FishTonguePreset"));
// 		capsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
// 		capsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
// 		capsuleComp->SetGenerateOverlapEvents(true);



	// 1. 기본 SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Underwater_life/Mesh/Skeletal_mesh/Animals/arapaima_rig_exp20_SK.arapaima_rig_exp20_SK'"));
 	if (tempMesh.Succeeded())
 	{
 		GetMesh()-> SetSkeletalMesh(tempMesh.Object);
 	}

	//Mesh 크기 세팅
	GetMesh()->SetRelativeScale3D(FVector(8));


	//2. 애니메이션 클래스 세팅
	ConstructorHelpers::FClassFinder<UFish_Anim> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrints/Fish_Anim/ABP_Fish.ABP_Fish_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	//3. FSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UFish_FSM>(TEXT("FSM"));
	
	//4. LineTrace 에 적용이 되게 세팅
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	//5.  AutoPossess AI 설정 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


}


	void AFish::BeginPlay()
	{
		Super::BeginPlay();

		//움직이는 방향으로 몸을 자동으로 회전하라는 옵션
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw =false;

		//랜덤 SkeletalMesh 담은 함수 호출
		RandMesh();

		//바뀔 때 순서
		GetMesh()->SetSkeletalMesh(nullptr);
		GetMesh()->SetAnimInstanceClass(nullptr);

		// 랜덤한 매쉬를 세팅
		GetMesh()->SetSkeletalMesh(arrayMesh[Rand]);

		// 랜덤한 애니메이션 세팅
		GetMesh()->SetAnimInstanceClass(FishAnimation[Rand]);

		// CapsuleComp에 overlap 되었을 때
		capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFish::Overlap);


		//물고기 - 머터리얼 인스턴스 
		fishMat = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);

 		//Outline - 머터리얼 인스턴스
		outlineMat = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
 
 		//Outline 색을 넣는다.
		GetMesh()-> SetMaterial (0, outlineMat);
		
	}


	void AFish::SetActive(bool bActive)
	{
		if (bActive)
		{
			GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::QueryAndPhysics));
			// 생성위치 재설정
			fsm->originPos = GetActorLocation();
		}
		else
		{
			// 충돌 비활성화
			GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
		}

		//  매쉬를 활성/ 비활성화
		GetMesh()->SetActive(bActive);
		// 매쉬 보이고/ 안보이고
		GetMesh()->SetVisibility(bActive);
		// 캐릭터 무브먼트 활성/ 비활성
		GetCharacterMovement()->SetActive(bActive);
		// fsm  활성/ 비활성
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

// 랜덤  SkeletalMesh 담을 함수
void AFish::RandMesh()
{
	//Skeletal Mesh 랜덤 재생
	Rand = FMath::RandRange(0, 9);

}

//Color off 함수
void AFish::ColorOff()
{
	// fishMat을 켜준다.
	GetMesh()->SetMaterial(0, fishMat);

}


void AFish::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != nullptr)
	{
		ABait* fishBait = Cast<ABait>(OtherActor);

		if (IsValid(fishBait))
		{
			fsm->ReceiveBait();
		}	
	}
	
}
