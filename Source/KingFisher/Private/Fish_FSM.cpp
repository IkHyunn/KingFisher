
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


	// SetActive (true/false) �۵��ǰ� ����.
	bAutoActivate = true;

	// ��Ÿ�� 
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

	//Ÿ�� ã��
	target = Cast<ABait>(UGameplayStatics::GetActorOfClass(GetWorld(), ABait::StaticClass()));

	//�� ã��
	me = Cast<AFish>(GetOwner());


	//�ʱ� ü�� ����
	currHP = maxHP;

	//ó�� ��ġ
	originPos = me->GetActorLocation();

	//animInstance ã��
	anim = Cast<UFish_Anim>(me->GetMesh()->GetAnimInstance());

	//ai ������ controller ���
	ai = Cast<AAIController>(me->GetController());



	// ���� �ý��� ********************* (������ ��)
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

 	// ���� �ý���_ ȣ�� **************ó�� �ɸ� ����⸸
	if (fishArray.Num() == 0)
	{
		return;
	}
	if (target->bBait)
	{
		//�̳��� �Դ� ù���� ����⸦ �迭�� ��� ��ġ�� ����.
		fishArray[0]->SetActorLocation();
		// �迭���� ù��° ����� Ȱ��ȭ
		fishArray[0]->SetActive(true);

		startLoc = target->GetActorLocation();
		endLoc = startLoc + me->GetActorLocation();

		ControlRotation(DeltaTime);
	}

	// �̳����� �ִܰŸ� ���ϱ�
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
// 	// Ÿ�ٰ��� �Ÿ��� ���Ѵ�.
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
// 				//NewDistace�� ���� �̳��� target���� �����Ѵ�.
// 				target = Cast<ABait>(targetclass[i]);
// 				min = distance;
// 
// 			}
// 		}
// 	}
//}



void UFish_FSM::UpdateSlowSwim()
{

	//�̳��� �Ѿư� �� �ֳ�
		if (IsWaitComplete(3))
		{
			// ���� ���¸� Swim���� �Ѵ�.
			ChangeState(EFishState::Swim);		
	 	}

}


void UFish_FSM::UpdateSwim()
{

		// �̳��� �Ѿư� �� �ֳ�
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

	// Ÿ�� ���� (dir)
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
// 	*********�߰� ************//
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
// 		// ����� �����ý���  *********************************
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

	//����⸦ �̳��� ���δ�.
	if (IsValid(target))
	{
		if (target->bBaitReady)
		{
			me->AttachToComponent(target->baitMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Tale"));
		}
		target->bBaitReady = false;

	}
	//���͸��� ���� ����
	me->ColorOff();

	//�߰�*****
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
//		//�÷��̾� ī�޶� ������ ���̰� �ʹ�.
// 		mywidget->AddToViewport();
// 		mywidget->Initialize();
// 	}
//}



bool UFish_FSM::IsTargetTrace()
{
	//�����->�̳� ���ϴ� ����
	FVector dirP = (target->GetActorLocation() - me->GetActorLocation());

	//2. ���� �չ���� 1���� ���� ������ ����
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dirP.GetSafeNormal());

	//3. 2���� ���� ����  ACos
	float angle = UKismetMathLibrary::DegAcos(dotValue);


	//��� 2- Sweep ���
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
		// �׷��� ������ false�� ��ȯ
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
		//����Ʈ���̽� ����
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
	// �ش� ��ġ��

	if (IsValid(ai))
	{
		EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

		//���� �������� �����Ѵٸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			ChangeState(EFishState::SlowSwim);
		}
	}
}

void UFish_FSM::UpdateReturnPos()
{
	// ó�� ��ġ�� ���� �����ϸ� Idle�� ��ȯ.
	MoveToPos(randPos);

}


void UFish_FSM::Navigation()
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	//������ ��ġ
	FNavLocation loc;
	ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
	randPos = loc.Location;
}


void UFish_FSM::ChangeState(EFishState state)
{

	//���� ���� ����
	currState = state;

	// ���� �ð� �ʱ�ȭ
	currTime = 0;

	// anim ���·� ����
	if (IsValid(anim))
	{
		anim->state = state;
	}

	// ai  ������ ������
	ai->StopMovement();


	//  ���¿� ���� �ʱ� ����
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

		//�Դ� ���� �ִϸ��̼� ���
		anim->bEatPlay = true;
		//�Դ� ���� ��ȯ ��, ��� �ð��� �ٷ� ��������
		currTime = EatDelayTime;

		// �̳� ���� �� (���� ���)
		int32 rand = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Eat%d"), rand);
		me->PlayAnimMontage(EatMontage, 1.0f, FName(*sectionName));

		UE_LOG(LogTemp, Warning, TEXT("EAT MONTAGE!!"));
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
		break;
	}
	}
}
