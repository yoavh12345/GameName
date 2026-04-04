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
enum class EActedOnField : uint8
{
	TransformZ UMETA(DisplayName = "Transform Z"),
	TransformX UMETA(DisplayName = "Transform X"),
	TransformY UMETA(DisplayName = "Transform Y")
};

UENUM(BlueprintType)
enum class EExpressionType : uint8
{
	Invalid UMETA(DisplayName = "Invalid"),
	Addition UMETA(DisplayName = "Addition"),
	Subtraction UMETA(DisplayName = "Subtraction"),
	Set UMETA(DisplayName = "Set")
};


USTRUCT(BlueprintType)
struct FCodeAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	EActedOnField ActedOnField;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	EExpressionType ExpressionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Code Editing")
	float Value;
};


UENUM(BlueprintType)
enum class ECodeEvent : uint8
{
	OnJump UMETA(DisplayName = "On Jump"),
	OnTakeDamage UMETA(DisplayName = "On Take Damage")
};

UENUM(BlueprintType)
enum class ECodeSyntaxErrorType : uint8
{
	InvalidExpression UMETA(DisplayName = "Invalid Expression"),
	InvalidValue UMETA(DisplayName = "Invalid Value"),
	UnsupportedField UMETA(DisplayName = "Unsupported Field"),
	InvalidAction UMETA(DisplayName = "Invalid Action")
};

USTRUCT(BlueprintType)
struct FCodeParseResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bSuccess = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<ECodeSyntaxErrorType> Errors;
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
	static FCodeParseResult HandleCodeChange(FString NewCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	static FCodeLineData ParseCodeLineData(FString OriginalCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE);
	
	static FString GetClassNameFromContext(TSubclassOf<AActor> Context);

private:
	static FCodeAction GetActionFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errs);

	static TArray<FString> GetExpressionsFromCode(FString Code, TArray<ECodeSyntaxErrorType>& Errs);

	static EExpressionType GetExpressionTypeFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errs);
	
	static bool doAction(FCodeAction Action, TSubclassOf<AActor> Context, UObject* WorldContextObject, ECodeEvent CE);
};
