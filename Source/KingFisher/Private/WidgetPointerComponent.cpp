// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPointerComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "FishPlayer.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>

// Sets default values for this component's properties
UWidgetPointerComponent::UWidgetPointerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWidgetPointerComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AFishPlayer>(GetOwner());
	
}


// Called every frame
void UWidgetPointerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWidgetPointerComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(widgetPointerInputs[0], ETriggerEvent::Started, this, &UWidgetPointerComponent::ClickWidget);
	PlayerInputComponent->BindAction(widgetPointerInputs[0], ETriggerEvent::Completed, this, &UWidgetPointerComponent::ReleaseWidget);
}

void UWidgetPointerComponent::ClickWidget()
{
	player->widgetPointer_Left->PressPointerKey(EKeys::LeftMouseButton);
}

void UWidgetPointerComponent::ReleaseWidget()
{
	player->widgetPointer_Left->ReleasePointerKey(EKeys::LeftMouseButton);
}

