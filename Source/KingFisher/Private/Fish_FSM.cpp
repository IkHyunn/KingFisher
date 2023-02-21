
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



	// 입질 시스템 ********************* (물었을 때)
//  	if (target != nullptr)
//  	{
// 		
// 		//myLoc = FVector::IsNormalized(-1*(target->GetActorLocation()- me->GetActorLocation()));
// 		//myLoc = FVector::GetSafeNormal(target->GetActorLocation() - me->GetActorLocation());
// 		//myLoc = target->GetActorLocation()*(-1.0f);
// 		
//  		startLoc = target->GetActorLocation();
//  		endLoc = startLoc + me->GetActorLocation();
// 
// 		
// 	}

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

 	// 입질 시스템_ 호출 **************처음 걸린 물고기만
	if (fishArray.Num() == 0)
	{
		return;
	}
	if (target->bBait)
	{
		//미끼를 먹는 첫번쨰 물고기를 배열에 담고 위치를 저장.
		fishArray[0]->SetActorLocation();
		// 배열에서 첫번째 물고기 활성화
		fishArray[0]->SetActive(true);

		startLoc = target->GetActorLocation();
		endLoc = startLoc + me->GetActorLocation();

		ControlRotation(DeltaTime);
	}

	// 미끼와의 최단거리 구하기
	/*FindDistance()*/;
}


void UFish_FSM::ControlRotation(float DeltaTime)
{
	currentTime += DeltaTime * direction;

	if (currentTime <= 0)
	{
		direction = 1;
	}

	if (currentTime >= 1)
	{
		direction = -1;
	}

	me->SetActorLocation(FMath::Lerp(startLoc, endLoc, currentTime));

}

// void UFish_FSM::FindDistance()
// {
// 	// 타겟과의 거리를 구한다.
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABait::StaticClass(), targetclass);
// 
// 	target = Cast<ABait>(targetclass[0]);
// 	min = FVector::Distance(targetclass[0]->GetActorLocation(), me->GetActorLocation());
// 
// 	for (int32 i = 1; i < targetclass.Num(); i++)
// 	{
// 		if (targetclass[i]->GetName().Contains(TEXT("Bait")))
// 		{
// 			UE_LOG(LogTemp,Warning, TEXT("%s"), *(targetclass[i]->GetName()));
// 			
// 			distance = FVector::Distance(targetclass[i]->GetActorLocation(), me->GetActorLocation());
// 
// 			if (min > distance)
// 			{
// 				//NewDistace를 가진 미끼를 target으로 설정한다.
// 				target = Cast<ABait>(targetclass[i]);
// 				min = distance;
// 
// 			}
// 		}
// 	}
//}



void UFish_FSM::UpdateSlowSwim()
{

	//미끼를 쫓아갈 수 있나
		if (IsWaitComplete(3))
		{
			// 현재 상태를 Swim으로 한다.
			ChangeState(EFishState::Swim);		
	 	}

}


void UFish_FSM::UpdateSwim()
{

		// 미끼를 쫓아갈 수 있나
		if (IsTargetTrace())
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
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());


	if (target->bBaitReady && !target->bBait)
	{
		if (dir.Length() > moveRange)
		{
			
			//ai->MoveToLocation(target->GetActorLocation());
			ai->MoveToLocation(target->GetActorLocation(), -1, false);
			

		}

	}
// 	else if (target->fish != me)
// 	{
// 		ChangeState(EFishState::ReturnPos);
// 	}

}


void UFish_FSM::UpdateEat()
{
// 	*********추가 ************//
// 		ChangeState(EFishState::Swim);
// 		UE_LOG(LogTemp, Warning, TEXT("UPDATE EATTT "));
}


void UFish_FSM::UpdateEatDelay()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > EatDelayTime)
	{
		currTime = 0;
		anim->bEatPlay = true;

		UE_LOG(LogTemp, Warning, TEXT("EAT DELAYYY"));

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
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
				ChangeState(EFishState::Swim);
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

// 		if (bBaitReady == false)
// 		{
// 		// 물고기 입질시스템  *********************************
// 		//ControlRotation(DeltatTime);

		//}
	}
	else
	{
		ChangeState(EFishState::Die);
	}
}

void UFish_FSM::UpdateDamaged()
{

	UE_LOG(LogTemp, Warning, TEXT("!!!!!! DAMAGE!!!!!!"));

	ChangeState(EFishState::Swim);
	target->bBaitReady = false;
	target->bBait = false;

}



void UFish_FSM::UpdateDie()
{

	//물고기를 미끼에 붙인다.
	if (IsValid(target))
	{
		if (target->bBaitReady)
		{
			me->AttachToComponent(target->baitMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Tale"));
		}
		target->bBaitReady = false;

	}
	//머터리얼 색을 꺼라
	me->ColorOff();

	//추가*****
	//bIsDead = true;

// 	if (bIsDead)
// 	{
// 		ShowWidget();
// 	}
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


// void UFish_FSM::ShowWidget()
// {
// 	FString WidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/Fish/BP_FishUI.BP_FishUI_C'");
// 	UUserWidget* mywidget = CreateWidget<UUserWidget>(GetWorld(),TEXT("WidgetPath"));
// 	if (mywidget)
// 	{
//		//플레이어 카메라에 위젯을 붙이고 싶다.
// 		mywidget->AddToViewport();
// 		mywidget->Initialize();
// 	}
//}



bool UFish_FSM::IsTargetTrace()
{
	//물고기->미끼 향하는 벡터
	FVector dirP = (target->GetActorLocation() - me->GetActorLocation());

	//2. 나의 앞방향과 1번에 구한 벡터의 내적
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dirP.GetSafeNormal());

	//3. 2번에 구한 값을  ACos
	float angle = UKismetMathLibrary::DegAcos(dotValue);


	//방법 2- Sweep 방식
// 	if (angle < 40 && dirP.Length() < traceRange)
// 	{
// 
// 		return true; 
// 	}

//*******************
// 	FVector StartLoc = me->GetActorLocation();
// 	FVector EndLoc = target->GetActorLocation();
// 	FHitResult hitInfo;
// 	FString profileName = FString (TEXT("DETECT"));
// 	FCollisionQueryParams params;
// 	params.AddIgnoredActor(me);
// 
// 	
// 	if (angle < 40 && dirP.Length() < traceRange)
// 	{
// 
// 	bHit = GetWorld()->SweepSingleByProfile(hitInfo, startLoc, endLoc, FQuat::Identity, TEXT("DETECT"),
// 		FCollisionShape::MakeSphere(Dest), params) ;
// 	
// 		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true, 2.0f, 0, 2.0f);
// 		if (bHit && target != nullptr)
// 		{	
// 			UE_LOG(LogTemp, Warning, TEXT("FAST SWIMMM"))
// 			if (hitInfo.GetActor()->GetName().Contains(TEXT("Bait")))
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("DETECTION!!!"))
// 				return true;
// 			}
// 		
// 		}
// 
//		
		// 그렇지 않으면 false로 전환
	//    return false;
// 	}
// 
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
	// 처음 위치로 가서 도착하면 Idle로 전환.
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
