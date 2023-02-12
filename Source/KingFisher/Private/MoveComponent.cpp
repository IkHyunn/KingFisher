// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "FishPlayer.h"
#include "InputAction.h"
#include <Components/CapsuleComponent.h>
#include "TeleportRing.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraDataInterfaceArrayFunctionLibrary.h>
#include "FIsherHandMesh.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AFishPlayer>(GetOwner());
	player->teleportTrace->SetVisibility(false);
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShowLine)
	{
		DrawMoveLine();
	}
}

void UMoveComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &UMoveComponent::RotateAxis);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &UMoveComponent::ShowLine);
	PlayerInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &UMoveComponent::HideLine);
	PlayerInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &UMoveComponent::MoveTeleportPos);
}

void UMoveComponent::RotateAxis(const struct FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();

	axis.Y = FMath::Clamp(axis.Y, -45, 45);
	player->AddControllerPitchInput(axis.Y * -1);

	player->AddControllerYawInput(axis.X);
}

void UMoveComponent::ShowLine()
{
	bShowLine = true;
}

void UMoveComponent::HideLine()
{
	bShowLine = false;
	Teleport();
}

void UMoveComponent::DrawMoveLine()
{
	float delayTime = 0.1;
	int32 lineCount = 100;
	
	FVector handForward = FRotationMatrix(player->rightHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(player->rightHand->GetComponentRotation()).GetUnitAxis(EAxis::X);

	FVector dir = handForward+handUp;

	lineLocation.Empty();

	for (int32 i = 0; i < lineCount; i++)
	{
		float timeTaken = delayTime * i;

		FVector position = player->rightHand->GetComponentLocation()+dir*power*timeTaken;
		position.Z += 0.5*GetWorld()->GetDefaultGravityZ()*timeTaken*timeTaken;

		lineLocation.Add(position);

		FHitResult hitinfo;

		if (i > 0)
		{
			if (GetWorld()->LineTraceSingleByChannel(hitinfo, lineLocation[i - 1], lineLocation[i], ECC_Visibility))
			{
				lineLocation.Add(hitinfo.ImpactPoint);
				break;
			}
		}
	}

// 	for (int32 i = 0; i < lineLocation.Num()-1; i++)
// 	{
// 		DrawDebugLine(GetWorld(),lineLocation[i], lineLocation[i+1], FColor::Red, false, -1, 0, 1.5);
// 	}

	player->teleportTrace->SetVisibility(true);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(player->teleportTrace, TEXT("User.PointArray"), lineLocation);

	if (spawnedSpot == nullptr)
	{
		spawnedSpot = GetWorld()->SpawnActor<ATeleportRing>(teleportSpot, lineLocation[lineLocation.Num()-1], FRotator::ZeroRotator);
	}
	else
	{
		spawnedSpot->teleportRingFX->SetVisibility(true);
		spawnedSpot->teleportBoundFX->SetVisibility(true);
		spawnedSpot->SetActorLocation(lineLocation[lineLocation.Num()-1]);
	}
}

void UMoveComponent::MoveTeleportPos(const struct FInputActionValue& value)
{
	float val = value.Get<float>();

	power += val*25;
	power = FMath::Clamp(power, 200, 500);
}

void UMoveComponent::Teleport()
{
	FVector targetLoc = lineLocation[lineLocation.Num()-1];
	targetLoc.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	player->SetActorLocation(targetLoc);

	if (spawnedSpot != nullptr)
	{
		spawnedSpot->teleportRingFX->SetVisibility(false);
		spawnedSpot->teleportBoundFX->SetVisibility(false);
		player->teleportTrace->SetVisibility(false);
	}
}