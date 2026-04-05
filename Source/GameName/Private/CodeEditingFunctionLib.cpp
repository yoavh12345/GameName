// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeEditingFunctionLib.h"
#include "Kismet/GameplayStatics.h"
#include "GameName/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

FCodeLineData UCodeEditingFunctionLib::ParseCodeLineData(FString OriginalCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE)
{
	FString ClassName = GetClassNameFromContext(Context);
	FString EventName = StaticEnum<ECodeEvent>()->GetNameStringByValue((int64)CE);
	FString UpperText = FString::Printf(TEXT("void %s::%s()\n{\n"), *ClassName, *EventName);
	FString LowerText = FString::Printf(TEXT("}\n"));
	FCodeLineData Data;
	Data.EditableCode = OriginalCodeLine;
	Data.UneditableUpperCode = UpperText;
	Data.UneditableLowerCode = LowerText;
	return Data;
}

FString UCodeEditingFunctionLib::GetClassNameFromContext(TSubclassOf<AActor> Context)
{
	if (!Context)
	{
		UE_LOG(LogTemp, Error, TEXT("Context is null"));
		return FString();
	}

	if (Context->IsChildOf(ATP_ThirdPersonCharacter::StaticClass()))
	{
		return "Player";
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Context is not supported!"));
		return FString();
	}

}

FCodeParseResult UCodeEditingFunctionLib::CompileRule(FString CodeLine, TSubclassOf<AActor> Context, ECodeEvent Event)
{
	FCodeParseResult ParseResult;
	ParseResult.Errors = TArray<ECodeSyntaxErrorType>();
	FCodeRule NewRule;
	NewRule.ContextClass = Context;
	NewRule.TriggerEvent = Event;
	NewRule.Operations = TArray<FCodeOperation>();
	TArray<FString> CodeLines;
	CodeLine.ParseIntoArrayLines(CodeLines);
	for (auto Line : CodeLines)
	{
		FCodeOperation Operation = ParseOperation(Line, ParseResult.Errors);
		NewRule.Operations.Add(Operation);
	}
	ParseResult.CompiledRule = NewRule;

	ParseResult.bSuccess = ParseResult.Errors.Num() <= 0;
	
	return ParseResult;
}

TArray<FString> UCodeEditingFunctionLib::TokenizeCode(FString Code)
{
	TArray<FString> Tokens;
	int32 i = 0;

	while (i < Code.Len())
	{
		TCHAR C = Code[i];

		// skip spaces
		if (FChar::IsWhitespace(C))
		{
			++i;
			continue;
		}

		// operators
		if (C == '+' || C == '-')
		{
			if (i + 1 < Code.Len() && Code[i + 1] == '=')
			{
				Tokens.Add(Code.Mid(i, 2));
				i += 2;
				continue;
			}
			else
			{
				++i;
				continue;
			}
		}

		if (C == '=')
		{
			Tokens.Add(TEXT("="));
			++i;
			continue;
		}

		// number literal (supports negative too if attached directly, like -500)
		if (FChar::IsDigit(C) || C == '.'
			|| (C == '-' && i + 1 < Code.Len() && (FChar::IsDigit(Code[i + 1]) || Code[i + 1] == '.')))
		{
			int32 Start = i;
			++i;

			while (i < Code.Len() && (FChar::IsDigit(Code[i]) || Code[i] == '.'))
			{
				++i;
			}

			Tokens.Add(Code.Mid(Start, i - Start));
			continue;
		}

		// identifier / property name
		int32 Start = i;
		while (i < Code.Len())
		{
			TCHAR Current = Code[i];

			bool bIsOperatorStart =
				Current == '=' ||
				Current == '+' ||
				Current == '-' ||
				FChar::IsWhitespace(Current);

			if (bIsOperatorStart)
			{
				break;
			}

			++i;
		}

		Tokens.Add(Code.Mid(Start, i - Start));
	}

	return Tokens;
}
FCodeOperation UCodeEditingFunctionLib::ParseOperation(FString Expression, TArray<ECodeSyntaxErrorType>& Errors)
{
	FCodeOperation NewOperation;
	TArray<FString> Tokenized = TokenizeCode(Expression);
	if (Tokenized.Num() != 3)
	{
		Errors.Add(ECodeSyntaxErrorType::InvalidAction);
		return NewOperation;
	}
	NewOperation.Property = GetPropertyFromExpression(Tokenized[0], Errors);
	NewOperation.Verb = GetVerbFromExpression(Tokenized[1], Errors);
	NewOperation.NumericValue = GetNumericValueFromExpression(Tokenized[2], Errors);
	NewOperation.TargetType = ECodeTargetType::Self;

	return NewOperation;
}

ECodeProperty UCodeEditingFunctionLib::GetPropertyFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors)
{
	Expression = Expression.ToLower();
	ECodeProperty ReturnProperty = ECodeProperty::Invalid;
	if (Expression == "transform.z")
		ReturnProperty = ECodeProperty::TransformZ;
	else if (Expression == "transform.x")
		ReturnProperty = ECodeProperty::TransformX;
	else if (Expression == "transform.y")
		ReturnProperty = ECodeProperty::TransformY;
	

	if (ReturnProperty == ECodeProperty::Invalid)
		Errors.Add(ECodeSyntaxErrorType::UnsupportedProperty);

	return ReturnProperty;
}

ECodeVerb UCodeEditingFunctionLib::GetVerbFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors)
{
	ECodeVerb ReturnVerb = ECodeVerb::Invalid;
	if (Expression == "=")
		ReturnVerb = ECodeVerb::Set;
	else if (Expression == "+=")
		ReturnVerb = ECodeVerb::Add;
	else if (Expression == "-=")
		ReturnVerb = ECodeVerb::Subtract;

	if (ReturnVerb == ECodeVerb::Invalid)
	{
		Errors.Add(ECodeSyntaxErrorType::UnsupportedVerb);
	}
	return ReturnVerb;
}

float UCodeEditingFunctionLib::GetNumericValueFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errors)
{
	TCHAR* EndPtr;
	double Result = std::wcstod(*Expression, &EndPtr);

	if (*EndPtr != '\0') {
		Errors.Add(ECodeSyntaxErrorType::InvalidValue);
	}
	return Result;
}

