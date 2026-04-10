// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeRuleSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "CodeEditableComponent.h"

bool UCodeRuleSubsystem::RegisterRule(const FCodeRule& Rule)
{
	ClearRulesForContext(Rule.ContextClass, Rule.TriggerEvent);
	CodeRules.Add(Rule);
	return true;
}

bool UCodeRuleSubsystem::RemoveRule(const FCodeRule& Rule)
{
	int32 Removed = CodeRules.RemoveAll([&](const FCodeRule& ExistingRule)
		{
			return ExistingRule.ContextClass == Rule.ContextClass &&
				ExistingRule.TriggerEvent == Rule.TriggerEvent;
		});

	return Removed > 0;
}

void UCodeRuleSubsystem::ClearRulesForContext(TSubclassOf<AActor> ContextClass, ECodeEvent Event)
{
	CodeRules.RemoveAll([&](const FCodeRule& Rule)
		{
			return Rule.ContextClass == ContextClass && Rule.TriggerEvent == Event;
		});
}

void UCodeRuleSubsystem::RaiseCodeEvent(ECodeEvent Event, const FCodeEventPayload& Payload)
{
	TArray<FCodeRule> MatchingRules = GetMatchingRules(Event, Payload.SourceActor);
	for (auto Rule : MatchingRules)
	{
		ExecuteRule(Rule, Payload);
	}
}

void UCodeRuleSubsystem::ExecuteRule(const FCodeRule& Rule, const FCodeEventPayload& Payload)
{
	for (auto Operation : Rule.Operations)
	{
		ExecuteOperation(Operation, Payload, Rule);
	}
}

TArray<FCodeRule> UCodeRuleSubsystem::GetMatchingRules(ECodeEvent Event, AActor* SourceActor) const
{
	TArray<FCodeRule> result;
	for (auto& Rule : CodeRules)
	{
		if (SourceActor)
		{
			if (SourceActor->GetClass()->IsChildOf(Rule.ContextClass) && Rule.TriggerEvent == Event)
				result.Add(Rule);
		}
	}

	return result;
}

void UCodeRuleSubsystem::ExecuteOperation(const FCodeOperation& Operation, const FCodeEventPayload& Payload, const FCodeRule& Rule)
{
	TArray<AActor*> Targets = ResolveTargets(Operation.TargetType, Rule, Payload);

	for (auto Target : Targets)
	{
		//to do: get component and call his ExecuteOperation method. 
		if (auto Comp = Target->GetComponentByClass(UCodeEditableComponent::StaticClass()))
		{
			auto CastedComp = Cast<UCodeEditableComponent>(Comp);
			if (!CastedComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("Target does not have a Code Editable Component"))
				return;
			}
			CastedComp->ApplyOperation(Operation, Payload);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Target does not have a Code Editable Component"))
		}
	}
}

TArray<AActor*> UCodeRuleSubsystem::ResolveTargets(ECodeTargetType TargetType, const FCodeRule& Rule, const FCodeEventPayload& Payload) const
{
	TArray<AActor*> Targets;
	if (TargetType == ECodeTargetType::AllOfContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("Getting all actors of class: %s"), *Rule.ContextClass->GetName())
		UGameplayStatics::GetAllActorsOfClass(this, Rule.ContextClass, Targets);
	}
	else if (TargetType == ECodeTargetType::Source && Payload.SourceActor)
	{
		Targets.Add(Payload.SourceActor);
	}
	else if (TargetType == ECodeTargetType::OtherActor && Payload.OtherActor)
	{
		Targets.Add(Payload.OtherActor);
	}
	else if (TargetType == ECodeTargetType::Self && Payload.SourceActor)
	{
		Targets.Add(Payload.SourceActor);
	}

	return Targets;
}
