
// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish_FSM.h"
#include "Kismet/GameplayStatics.h"
#include "Fish.h"
#include "Fish_Anim.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "AIModule/Classes/AIController.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "Bait.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "FishPlayer.h"
//#include "UObject/Class.h"
//#include "AIModule/Classes/Navigation/PathFollowingComponent.h"


// Sets default values for this component's properties
UFish_FSM::UFish_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// SetActive (true/false) 작동되게 하자.
	bAutoActivate = true;

	// 몽타주 
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/BluePrints/Fish_Anim/Retargeted_Anim/Retargeted_AMT/IKG_arapaimaAMT_arapaima.IKG_arapaimaAMT_arapaima'"));
	if (tempMontage.Succeeded())
	{
		EatMontage = tempMontage.Object;
	}

}


// Called when the game starts
void UFish_FSM::BeginPlay()
{
	Super::BeginPlay();

	//타겟 찾기
	target = Cast<ABait>(UGameplayStatics::GetActorOfClass(GetWorld(), ABait::StaticClass()));

	//나 찾기
	me = Cast<AFish>(GetOwner());


	//초기 체력 세팅
	currHP = maxHP;

	//처음 위치
	originPos = me->GetActorLocation();

	//animInstance 찾기
	anim = Cast<UFish_Anim>(me->GetMesh()->GetAnimInstance());

	//ai 변수에 controller 담기
	ai = Cast<AAIController>(me->GetController());

	fishPlayer = Cast<AFishPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));



}

// Called every frame
void UFish_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	switch (currState)
	{
	case EFishState::SlowSwim:
		UpdateSlowSwim();
		break;
	case EFishState::Swim:
		UpdateSwim();
		break;
	case EFishState::FastSwim:
		UpdateFastSwim();
		break;
	case EFishState::Eat:
		UpdateEat();
		break;
	case EFishState::EatDelay:
		UpdateEatDelay();
		break;
	case EFishState::Damaged:
		UpdateDamaged();
		break;
	case EFishState::Die:
		UpdateDie();
		break;
	case EFishState::ReturnPos:
		UpdateReturnPos();
		break;
	}


	// 미끼와의 최단거리 구하기
	FindDistance();


	// 입질 시스템 
	if (target != nullptr)
	{
		if (me->bBait && currState != EFishState::Die)
		{
		//매개변수 DeltaTime으로 속도 조절
			BiteSystem(0.004);
	
		}
		// 미끼에 닿지 않았다면
		else
		{
			
			//ChangeState(EFishState::SlowSwim);
		}
	}
}

// 입질시스템
void UFish_FSM::BiteSystem(float DeltaTime)
{
	

	//시작 위치
	startLoc = target->GetActorLocation();
	//종료 위치
	endLoc = startLoc + me->GetActorLocation();


	currentTime += DeltaTime * direction;
	
		if (currentTime <= 0)
		{
			direction = 1;
		}
	
		if (currentTime >= 1)
		{
			direction = -1;
		}
	
		me->SetActorLocation(FMath::Lerp(startLoc, endLoc, currentTime/6));

	



}

// 물고기가 미끼와의 최단거리 구하는 함수
void UFish_FSM::FindDistance()
{
	// 타겟과의 거리를 구한다.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABait::StaticClass(), targetclass);

	target = Cast<ABait>(targetclass[0]);
	min = FVector::Distance(targetclass[0]->GetActorLocation(), me->GetActorLocation());
	int32 num = 0;

	for (int32 i = 1; i < targetclass.Num(); i++)
	{
		if (targetclass[i]->GetName().Contains(TEXT("Bait")))
		{

			distance = FVector::Distance(targetclass[i]->GetActorLocation(), me->GetActorLocation());

			if (min > distance)
			{
				//NewDistace를 가진 미끼를 target으로 설정한다.
				target = Cast<ABait>(targetclass[i]);
				min = distance;
				num = i;
			}
		}
	}


}

 

void UFish_FSM::UpdateSlowSwim()
{

	//미끼를 쫓아갈 수 있나
		if (IsWaitComplete(4))
		{
			// 현재 상태를 Swim으로 한다.
			ChangeState(EFishState::Swim);		
	 	}

}


void UFish_FSM::UpdateSwim()
{
		bTrace = IsTargetTrace();

		// 미끼를 쫓아갈 수 있나
		if (bTrace)
		{
			ChangeState(EFishState::FastSwim);
		}
		else
		{
			MoveToPos(randPos);
		}

}



void UFish_FSM::UpdateFastSwim()
{

	// 타겟 방향 (dir)
	dir = target->GetActorLocation() - me->GetActorLocation();
	float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());


	if (target->bBaitReady && !target->bBait)
	{
		if (dir.Length() > moveRange)
		{	
			//ai->MoveToLocation(target->GetActorLocation());
			ai->MoveToLocation(target->GetActorLocation(), -1, false);	

			if (dir.Length() < eatableRange)
			{
				//상태를 Attack 으로 변경
				ChangeState(EFishState::Eat);
			}
		}
	}

}


void UFish_FSM::UpdateEat()
{
	//ChangeState(EFishState::EatDelay);
}


void UFish_FSM::UpdateEatDelay()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > EatDelayTime)
	{
		currTime = 0;

		UE_LOG(LogTemp, Warning, TEXT("EAT DELAYYY Timeee"));

		 dir = target->GetActorLocation() - me->GetActorLocation();

		if (dir.Length() < eatableRange)
		{
			bBite = true;

			if (bBite)
			{
				ChangeState(EFishState::Eat);
			}
		}
		else
		{
			if (bBite == false)
			{
				ChangeState(EFishState::SlowSwim);
			}
		}
	}
}


void UFish_FSM::ReceiveBait()
{
	currHP--;
	if (currHP > 0)
	{
		ChangeState(EFishState::Damaged);

	}
	else
	{
		ChangeState(EFishState::Die);
		fishPlayer->bCatch = true;
	}
}

void UFish_FSM::UpdateDamaged()
{

	UE_LOG(LogTemp, Warning, TEXT("!!!!!! DAMAGE!!!!!!"));

//  	currTime += GetWorld()->DeltaTimeSeconds;
//  	if (currTime > EatDelayTime)
//  	{
	
	//왔다갔다 Lerp 넣는다.
	ChangeState(EFishState::Swim);

	currTime = 0;
	target->bBaitReady = false;
	target->bBait = false;

	//}


}

void UFish_FSM::UpdateDie()
{
	//물고기를 미끼에 붙인다.
	if (IsValid(target))
	{

		me->AttachToComponent(target->baitMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Tale"));
		target->bBaitReady = false;
	}
	//머터리얼 색을 꺼라
	me->ColorOff();

	
}


bool UFish_FSM::IsWaitComplete(float delayTime)
{
	currTime += GetWorld()->DeltaTimeSeconds;

	if (currTime > delayTime)
	{
		currTime = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool UFish_FSM::IsTargetTrace()
{
	//물고기->미끼 향하는 벡터
	FVector dirP = (target->GetActorLocation() - me->GetActorLocation());

	//2. 나의 앞방향과 1번에 구한 벡터의 내적
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dirP.GetSafeNormal());

	//3. 2번에 구한 값을  ACos
	float angle = UKismetMathLibrary::DegAcos(dotValue);


	//Linetrace
	FVector StartLoc = me->GetActorLocation();
	FVector EndLoc = target->GetActorLocation();

	FHitResult hitInfo;
	FCollisionQueryParams TraceParams(TEXT("LineTrace"), true, GetOwner());
	TraceParams.bTraceComplex = true; 
	TraceParams.bReturnPhysicalMaterial = true; 

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, StartLoc, EndLoc, ECC_Visibility, TraceParams))
	{
		//라인트레이스 성공
		ABait* HitActor = Cast<ABait>(hitInfo.GetActor());
		
		if (HitActor)
		{
			DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true, 2.0f, 0, 2.0f);

			return true;
		}

	}
			return false;

}


void UFish_FSM::MoveToPos(FVector pos)
{
	// 해당 위치로

	if (IsValid(ai))
	{
		EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

		//만약 목적지에 도착한다면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			ChangeState(EFishState::SlowSwim);
		}
	}
}

void UFish_FSM::UpdateReturnPos()
{
	// 처음 위치로 가서 도착하면 SlowSwim로 전환.
	MoveToPos(randPos);

}


void UFish_FSM::Navigation()
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	//랜덤한 위치
	FNavLocation loc;
	ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
	randPos = loc.Location;
}


void UFish_FSM::ChangeState(EFishState state)
{

	//현재 상태 갱신
	currState = state;

	// 현재 시간 초기화
	currTime = 0;

	// anim 상태로 갱신
	if (IsValid(anim))
	{
		anim->state = state;
	}

	// ai  움직임 멈추자
	ai->StopMovement();


	//  상태에 따른 초기 설정
	switch (currState)
	{
	case EFishState::SlowSwim:
	{
		Navigation();
	}
	break;

	case EFishState::Swim:
	{
		Navigation();
	}
	break;

	case EFishState::FastSwim:
	{
		Navigation();

	}
	break;

	case EFishState::Eat:
	{

		//먹는 상태 애니메이션 재생
		anim->bEatPlay = true;
		//먹는 상태 전환 시, 대기 시간이 바로 끝나도록
		currTime = EatDelayTime;

		// 미끼 먹을 때 (랜덤 재생)
		int32 rand = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Eat%d"), rand);
		me->PlayAnimMontage(EatMontage, 1.0f, FName(*sectionName));

		UE_LOG(LogTemp, Warning, TEXT("EAT MONTAGE!!"));
	}
	break;

	case EFishState::Die:
	{
		//충돌 안되게
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 죽음 몽타주 (랜덤 재생)
		int32 rand = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Die%d"), rand);
		me->PlayAnimMontage(EatMontage, 1.0f, FName(*sectionName));
		break;
	}
	}
}
