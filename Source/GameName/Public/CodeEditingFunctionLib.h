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
	None            UMETA(DisplayName = "None"),
	OnJump          UMETA(DisplayName = "On Jump"),
	OnCodeChange		UMETA(DisplayName = "OnCodeChange"),
	OnHit			UMETA(DisplayName = "On Hit")
};

UENUM(BlueprintType)
enum class ECodeVerb : uint8
{
	Invalid         UMETA(DisplayName = "Invalid"),
	Set             UMETA(DisplayName = "Set"),
	Add             UMETA(DisplayName = "Add"),
	Subtract        UMETA(DisplayName = "Subtract")
};

UENUM(BlueprintType)
enum class ECodeProperty : uint8
{
	Invalid         UMETA(DisplayName = "Invalid"),
	TransformX      UMETA(DisplayName = "Transform X"),
	TransformY      UMETA(DisplayName = "Transform Y"),
	TransformZ      UMETA(DisplayName = "Transform Z"),
	Health          UMETA(DisplayName = "Health"),
	JumpVelocity    UMETA(DisplayName = "Jump Velocity"),
	Speed			UMETA(DisplayName = "Speed")
};

UENUM(BlueprintType)
enum class ECodeTargetType : uint8
{
	Source          UMETA(DisplayName = "Source"),
	OtherActor      UMETA(DisplayName = "Other Actor"),
	AllOfContext    UMETA(DisplayName = "All Of Context"),
	Self            UMETA(DisplayName = "Self")
};

UENUM(BlueprintType)
enum class ECodeSyntaxErrorType : uint8
{
	None                    UMETA(DisplayName = "None"),
	InvalidExpression       UMETA(DisplayName = "Invalid Expression"),
	InvalidValue            UMETA(DisplayName = "Invalid Value"),
	UnsupportedProperty     UMETA(DisplayName = "Unsupported Property"),
	UnsupportedVerb         UMETA(DisplayName = "Unsupported Verb"),
	InvalidTarget           UMETA(DisplayName = "Invalid Target"),
	InvalidEvent            UMETA(DisplayName = "Invalid Event"),
	InvalidAction           UMETA(DisplayName = "Invalid Action"),
	UnknownIdentifier       UMETA(DisplayName = "Unknown Identifier")
};


USTRUCT(BlueprintType)
struct FCodeOperation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECodeVerb Verb = ECodeVerb::Invalid;

	UPBROPERTY(BlueprintReadWrite, EditAnywhere)
	ECodeProperty Property = ECodeProperty::Invalid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECodeTargetType TargetType = ECodeTargetType::Source;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float NumericValue = 0.f;
};

USTRUCT(BlueprintType)
struct FCodeRule
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECodeEvent TriggerEvent = ECodeEvent::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ContextClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCodeOperation> Operations;
};


USTRUCT(BlueprintType)
struct FCodeEventPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Initiator = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* SourceActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* OtherActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float NumericValue = 0.f;
};

USTRUCT(BlueprintType)
struct FCodeParseResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bSuccess = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<ECodeSyntaxErrorType> Errors;

	UPROPERTY(BlueprintReadOnly)
	FCodeRule CompiledRule;
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
	static FCodeLineData ParseCodeLineData(FString OriginalCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE);
	
	static FString GetClassNameFromContext(TSubclassOf<AActor> Context);

	UFUNCTION(BlueprintCallable, Category = "Code Editing")
	static FCodeParseResult CompileRule(FString CodeLine, TSubclassOf<AActor> Context, ECodeEvent Event);
private:

	static TArray<FString> TokenizeCode(FString Code);

	static FCodeOperation ParseOperation(FString Expression, TArray<ECodeSyntaxErrorType>& Errors);

	static ECodeProperty GetPropertyFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors);
	
	static ECodeVerb GetVerbFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors);

	static float GetNumericValueFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors);
	
	static ECodeTargetType GetTargetTypeFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors);

	UFUNCTION(BlueprintPure, Category = "TextValidation")
	static bool IsTextChangeValid(const FString& Original, const FString& Current, int32 MaxEdits, int32& OutEditsLeft);
};
