// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeEditableComponent.h"

// Sets default values for this component's properties
UCodeEditableComponent::UCodeEditableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCodeEditableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCodeEditableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCodeEditableComponent::ApplyOperation_Implementation(const FCodeOperation& Operation, const FCodeEventPayload& Payload)
{
	if (!SupportsVerb(Operation.Verb)) return false;

	if (Operation.Property != ECodeProperty::Invalid)
	{
		if (!SupportsProperty(Operation.Property)) return false;
		return ApplyNumericProperty(Operation.Verb, Operation.Property, Operation.NumericValue);
	}

	return ApplySpecialVerb(Operation.Verb, Operation.NumericValue, Payload);
}

bool UCodeEditableComponent::SupportsProperty(ECodeProperty Property) const
{
	return SupportedProperties.Contains(Property);
}

bool UCodeEditableComponent::SupportsVerb(ECodeVerb Verb) const
{
	return SupportedVerbs.Contains(Verb);
}

