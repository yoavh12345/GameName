// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CodeEditingFunctionLib.h"
#include "CodeRuleSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GAMENAME_API UCodeRuleSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	bool RegisterRule(const FCodeRule& Rule);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	bool RemoveRule(const FCodeRule& Rule);
	
	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	void ClearRulesForContext(TSubclassOf<AActor> ContextClass, ECodeEvent Event);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	void RaiseCodeEvent(ECodeEvent Event, const FCodeEventPayload& Payload);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	void ExecuteRule(const FCodeRule& Rule, const FCodeEventPayload& Payload);

	TArray<FCodeRule> GetMatchingRules(ECodeEvent Event, AActor* SourceActor) const;



private:

	TArray<FCodeRule> CodeRules;

	void ExecuteOperation(const FCodeOperation& Operation, const FCodeEventPayload& Payload, const FCodeRule& Rule);

	TArray<AActor*> ResolveTargets(ECodeTargetType TargetType, const FCodeRule& Rule, const FCodeEventPayload& Payload) const;
};
