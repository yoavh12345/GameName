// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeEventComponent.h"
#include "CodeRuleSubsystem.h"

// Sets default values for this component's properties
UCodeEventComponent::UCodeEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCodeEventComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCodeEventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCodeEventComponent::RaiseEvent(ECodeEvent EventType, const FCodeEventPayload& Payload)
{
	UCodeRuleSubsystem* Subsys = GetCodeSubsystem();
	Subsys->RaiseCodeEvent(EventType, Payload);
}

UCodeRuleSubsystem* UCodeEventComponent::GetCodeSubsystem() const
{
	return GetWorld()->GetSubsystem<UCodeRuleSubsystem>();
}

