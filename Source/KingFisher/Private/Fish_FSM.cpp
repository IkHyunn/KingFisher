// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish_FSM.h"
#include "Kismet/GameplayStatics.h"
#include "Fish.h"
#include "Fish_Anim.h"
#include "FishBait.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "AIModule/Classes/AIController.h"
#include "NavigationSystem/Public/NavigationSystem.h"
//#include "UObject/Class.h"
//#include "AIModule/Classes/Navigation/PathFollowingComponent.h"


// Sets default values for this component's properties
UFish_FSM::UFish_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//  SetActive (true/false) 작동되게 하자.
	bAutoActivate = true;

	// 몽타주 
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/BluePrints/Fish_Anim/AMT_arapaima.AMT_arapaima'"));
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
		target = Cast<AFishBait>(UGameplayStatics::GetActorOfClass(GetWorld(), AFishBait::StaticClass()));
		//나 찾기
		me = Cast<AFish>(GetOwner());
		//처음 위치
		originPos = me->GetActorLocation();
		//animInstance 찾기
		anim = Cast<UFish_Anim>(me->GetMesh()->GetAnimInstance());
		// ai 변수에  AI_controller 찾아서 담기
		ai = Cast<AAIController>(me->GetController());


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
}

	void UFish_FSM::UpdateSlowSwim()
	{
		//  미끼를 쫓아갈 수 있나
		if (IsTargetTrace())
			{
			// 현재 상태를 Swim으로 한다.
			ChangeState(EFishState::Swim);
			}
		else
			{
			//idleDelayTime이 지나면
			bool bComplete = IsWaitComplete(idleDelayTime);
			if (bComplete == true)
				{
				// 현재 상태를 Swim 으로 한다.
				ChangeState(EFishState::Swim);
				}
			}
	}


	void UFish_FSM::UpdateSwim()
	{
		// 타겟 방향 (dir)
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		// 처음위치, 나(물고기) 현재 위치와의 거리 (dist)
		float dist = FVector::Distance(originPos, me->GetActorLocation());
		// 미끼가 시야에 들어왔는지
		bool bTrace = IsTargetTrace();


		//민약 거리( dist) 가 moveRange 보다 커지면 (움직이는 반경 넘어간다면)
		if (dist > moveRange)
			{
			//상태를 returnPos로 변경
			ChangeState(EFishState::ReturnPos);
			}

		//만약 미끼가 시야에 들어왔다면
		else if (bTrace)
		{
			//만약 target과 나(물고기)의 거리가 먹을 수 있는 거리보다 작으면
			if (dir.Length() < feedRange)
			{
				// 상태를  Eat 으로 변경
				ChangeState(EFishState::FastSwim);

				//먹을 수 있는 거리로 바뀌면, 상태를 eat으로 전환.
				if (dir.Length() < eatableRange)
					{
						ChangeState(EFishState::Eat);

						//먹는 상태 애니메이션 재생
						anim->bEatPlay = true;

						//먹는 상태 전환 시, 대기 시간이 바로 끝나도록
						currTime = EatDelayTime;
					}
			}
			//그렇지 않으면
 			else
 			{
 				// ai 이용해서 목적지까지 이동
 				ai->MoveToLocation(target->GetActorLocation());
 			}
		}

		//시야에 들어오지 않았다면
		else
			{
			//랜덤한 위치로 도착 후 Idle로 전환
			MoveToPos(randPos);
			}
		
	}
	void UFish_FSM::UpdateFastSwim()
	{
		//ChangeState(EFishState::FastSwim);
	}

	void UFish_FSM::UpdateEat()
	{
		UE_LOG(LogTemp, Warning, TEXT("EAT!"));
		ChangeState(EFishState::EatDelay);
	}


	void UFish_FSM::UpdateEatDelay()
	{
		currTime += GetWorld()->DeltaTimeSeconds;
		if (currTime > EatDelayTime)
		{
			currTime = 0;
			anim->bEatPlay = true;

			FVector dir = target->GetActorLocation() - me->GetActorLocation();
			if (dir.Length() < feedRange)
			{
				// 미끼 범위안에 들면 천천히 다가온다.
				ChangeState(EFishState::SlowSwim);
			}
			else
			{
				ChangeState(EFishState::Swim);
			}
		}
	}

	void UFish_FSM::UpdateDamaged()
	{
		//delayTime이 지나면
		if (IsWaitComplete(damageDelayTime))
		{
			ChangeState(EFishState::SlowSwim);
		}
	}

	void UFish_FSM::UpdateDie()
	{
		// 만약 bDieMove가  false라면 함수를 나가라

		//  미끼를 물었을 떄 상태를  FastSwim으로 바꿔라
		//me->PlayAnimMontage(EatMontage, 1.0f, FName("Die1"));

		// 미끼에서 제거되었을 떄 상태를 Idle로 바꿔라
		//me->PlayAnimMontage(EatMontage, 1.0f, FName("Die0"));

	}

	void UFish_FSM::UpdateReturnPos()
	{
		// 처음 위치로 가서 도착하면 Idle로 전환.
		MoveToPos(originPos);
	
		//1. 나--> 처음 위치를 향하는 방향
	 	FVector dir = originPos - me->GetActorLocation();
	 	
	 	//2. 만약 나---처음 위치까지 거리가 10보다 작으면
	 	if (dir.Length() < 10 )
	 		{
	 			//3. Idle 상태로 전환
	 			ChangeState(EFishState::SlowSwim);
	 		}
	 	//4. 그렇지 않으면
	 	else
	 		{
	 			//5. 계속 1번에서 구한 방향으로 이동
	 			me->AddMovementInput(dir.GetSafeNormal());
	 		}
	}

	void UFish_FSM::ChangeState(EFishState state)
	{
		
			//현재 상태 갱신
			currState = state;
			// 현재 시간 초기화
 			currTime = 0;
 			// anim 상태로 갱신
 			anim->state = state;
 			// ai  움직임 멈추자
 			ai->StopMovement();


		//  상태에 따른 초기 설정
		switch(currState)
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
					// 미끼 먹을 때 (랜덤 재생)
					int32 rand = FMath::RandRange(0,1);
					FString sectionName = FString::Printf(TEXT("Eat%d"), rand);
					me->PlayAnimMontage(EatMontage, 1.0f, FName(*sectionName));
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
				}
			break;
		}
	}

	void UFish_FSM::ReceiveBait()
	{
		currHP--;
			if(currHP>0)
				{
					ChangeState(EFishState::Damaged);
				}
			else
				{
					ChangeState(EFishState::Die);
				}
	}


	bool UFish_FSM::IsWaitComplete(float delayTime)
	{
		currTime +=GetWorld()->DeltaTimeSeconds;

			if (currTime>delayTime)
					{
						currTime =0;
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
		 FVector dirP = (target->GetActorLocation()-me->GetActorLocation());

		 //2. 나의 앞방향과 1번에 구한 벡터의 내적
		  float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dirP.GetSafeNormal());

		  //3. 2번에 구한 값을  ACos
		  float angle = UKismetMathLibrary::DegAcos(dotValue);


		  if (angle <40 && dirP.Length() < traceRange)
		  {
			//LineTrace
			FHitResult hitInfo;
			FCollisionQueryParams param;
			param.AddIgnoredActor(me);

			bool bHit = GetWorld()->LineTraceSingleByChannel(
			hitInfo,
			me->GetActorLocation(),
			target->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			param);

			// 만약 부딪힌 곳 있다면
			 if(bHit)
			 {
				if(hitInfo.GetActor()->GetName().Contains(TEXT("Bait")))
				{
					return true;
				}
			 }

		  }
			// 그렇지 않으면 false로 전환
				 return false;
	}


	void UFish_FSM::MoveToPos(FVector pos)
	{
 		// 해당 위치로
 		 EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);
 
 		//만약 목적지에 도착한다면
 		if(result == EPathFollowingRequestResult::AlreadyAtGoal)
 			{	
 				ChangeState(EFishState::SlowSwim);
 			}

	}

	void UFish_FSM::Navigation()
	{
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
