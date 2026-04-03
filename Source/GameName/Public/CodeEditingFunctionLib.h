// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CodeEditingFunctionLib.generated.h"

USTRUCT(BlueprintType)
struct FCodeLineData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	FString UneditableUpperCode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	FString EditableCode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	FString UneditableLowerCode;
};

UENUM(BlueprintType)
enum class ECodeEvent : uint8
{
	OnJump UMETA(DisplayName = "On Jump"),
	OnTakeDamage UMETA(DisplayName = "On Take Damage")
};

/**
 * 
 */
UCLASS()
class GAMENAME_API UCodeEditingFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	static void HandleCodeChange(FString NewCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	static FCodeLineData ParseCodeLineData(FString OriginalCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE);
	
	static FString GetClassNameFromContext(TSubclassOf<AActor> Context);
};
