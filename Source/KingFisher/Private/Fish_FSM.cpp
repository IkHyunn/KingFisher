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


	//  SetActive (true/false) �۵��ǰ� ����.
	bAutoActivate = true;

	// ��Ÿ�� 
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

		//Ÿ�� ã��
		target = Cast<AFishBait>(UGameplayStatics::GetActorOfClass(GetWorld(), AFishBait::StaticClass()));
		//�� ã��
		me = Cast<AFish>(GetOwner());
		//ó�� ��ġ
		originPos = me->GetActorLocation();
		//animInstance ã��
		anim = Cast<UFish_Anim>(me->GetMesh()->GetAnimInstance());
		// ai ������  AI_controller ã�Ƽ� ���
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
		//  �̳��� �Ѿư� �� �ֳ�
		if (IsTargetTrace())
			{
			// ���� ���¸� Swim���� �Ѵ�.
			ChangeState(EFishState::Swim);
			}
		else
			{
			//idleDelayTime�� ������
			bool bComplete = IsWaitComplete(idleDelayTime);
			if (bComplete == true)
				{
				// ���� ���¸� Swim ���� �Ѵ�.
				ChangeState(EFishState::Swim);
				}
			}
	}


	void UFish_FSM::UpdateSwim()
	{
		// Ÿ�� ���� (dir)
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		// ó����ġ, ��(�����) ���� ��ġ���� �Ÿ� (dist)
		float dist = FVector::Distance(originPos, me->GetActorLocation());
		// �̳��� �þ߿� ���Դ���
		bool bTrace = IsTargetTrace();


		//�ξ� �Ÿ�( dist) �� moveRange ���� Ŀ���� (�����̴� �ݰ� �Ѿ�ٸ�)
		if (dist > moveRange)
			{
			//���¸� returnPos�� ����
			ChangeState(EFishState::ReturnPos);
			}

		//���� �̳��� �þ߿� ���Դٸ�
		else if (bTrace)
		{
			//���� target�� ��(�����)�� �Ÿ��� ���� �� �ִ� �Ÿ����� ������
			if (dir.Length() < feedRange)
			{
				// ���¸�  Eat ���� ����
				ChangeState(EFishState::FastSwim);

				//���� �� �ִ� �Ÿ��� �ٲ��, ���¸� eat���� ��ȯ.
				if (dir.Length() < eatableRange)
					{
						ChangeState(EFishState::Eat);

						//�Դ� ���� �ִϸ��̼� ���
						anim->bEatPlay = true;

						//�Դ� ���� ��ȯ ��, ��� �ð��� �ٷ� ��������
						currTime = EatDelayTime;
					}
			}
			//�׷��� ������
 			else
 			{
 				// ai �̿��ؼ� ���������� �̵�
 				ai->MoveToLocation(target->GetActorLocation());
 			}
		}

		//�þ߿� ������ �ʾҴٸ�
		else
			{
			//������ ��ġ�� ���� �� Idle�� ��ȯ
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
				// �̳� �����ȿ� ��� õõ�� �ٰ��´�.
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
		//delayTime�� ������
		if (IsWaitComplete(damageDelayTime))
		{
			ChangeState(EFishState::SlowSwim);
		}
	}

	void UFish_FSM::UpdateDie()
	{
		// ���� bDieMove��  false��� �Լ��� ������

		//  �̳��� ������ �� ���¸�  FastSwim���� �ٲ��
		//me->PlayAnimMontage(EatMontage, 1.0f, FName("Die1"));

		// �̳����� ���ŵǾ��� �� ���¸� Idle�� �ٲ��
		//me->PlayAnimMontage(EatMontage, 1.0f, FName("Die0"));

	}

	void UFish_FSM::UpdateReturnPos()
	{
		// ó�� ��ġ�� ���� �����ϸ� Idle�� ��ȯ.
		MoveToPos(originPos);
	
		//1. ��--> ó�� ��ġ�� ���ϴ� ����
	 	FVector dir = originPos - me->GetActorLocation();
	 	
	 	//2. ���� ��---ó�� ��ġ���� �Ÿ��� 10���� ������
	 	if (dir.Length() < 10 )
	 		{
	 			//3. Idle ���·� ��ȯ
	 			ChangeState(EFishState::SlowSwim);
	 		}
	 	//4. �׷��� ������
	 	else
	 		{
	 			//5. ��� 1������ ���� �������� �̵�
	 			me->AddMovementInput(dir.GetSafeNormal());
	 		}
	}

	void UFish_FSM::ChangeState(EFishState state)
	{
		
			//���� ���� ����
			currState = state;
			// ���� �ð� �ʱ�ȭ
 			currTime = 0;
 			// anim ���·� ����
 			anim->state = state;
 			// ai  ������ ������
 			ai->StopMovement();


		//  ���¿� ���� �ʱ� ����
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
					// �̳� ���� �� (���� ���)
					int32 rand = FMath::RandRange(0,1);
					FString sectionName = FString::Printf(TEXT("Eat%d"), rand);
					me->PlayAnimMontage(EatMontage, 1.0f, FName(*sectionName));
				}
			break;

			case EFishState::Die:
				{
					//�浹 �ȵǰ�
					 me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					// ���� ��Ÿ�� (���� ���)
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
		//�����->�̳� ���ϴ� ����
		 FVector dirP = (target->GetActorLocation()-me->GetActorLocation());

		 //2. ���� �չ���� 1���� ���� ������ ����
		  float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dirP.GetSafeNormal());

		  //3. 2���� ���� ����  ACos
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

			// ���� �ε��� �� �ִٸ�
			 if(bHit)
			 {
				if(hitInfo.GetActor()->GetName().Contains(TEXT("Bait")))
				{
					return true;
				}
			 }

		  }
			// �׷��� ������ false�� ��ȯ
				 return false;
	}


	void UFish_FSM::MoveToPos(FVector pos)
	{
 		// �ش� ��ġ��
 		 EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);
 
 		//���� �������� �����Ѵٸ�
 		if(result == EPathFollowingRequestResult::AlreadyAtGoal)
 			{	
 				ChangeState(EFishState::SlowSwim);
 			}

	}

	void UFish_FSM::Navigation()
	{
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
