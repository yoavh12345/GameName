// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CodeEditingFunctionLib.h"
#include "CodeEditableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class GAMENAME_API UCodeEditableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCodeEditableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Code Editing")
	bool ApplyOperation(const FCodeOperation& Operation, const FCodeEventPayload& Payload);

	virtual bool ApplyOperation_Implementation(const FCodeOperation& Operation, const FCodeEventPayload& Payload);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	bool SupportsProperty(ECodeProperty Property) const;

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	bool SupportsVerb(ECodeVerb Verb) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Code Editing")
	bool ApplyNumericProperty(ECodeVerb Verb, ECodeProperty Property, float Value);

	bool ApplyNumericProperty_Implementation(ECodeVerb Verb, ECodeProperty Property, float Value) { return false; };

	UFUNCTION(BlueprintNativeEvent, Category = "Code Editing")
	bool ApplySpecialVerb(ECodeVerb Verb, float Value, const FCodeEventPayload& Payload);

	virtual	bool ApplySpecialVerb_Implementation(ECodeVerb Verb, float Value, const FCodeEventPayload& Payload) { return false; };

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	TArray<ECodeVerb> SupportedVerbs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	TArray<ECodeProperty> SupportedProperties;
};
