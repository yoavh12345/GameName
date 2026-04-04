// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeEditingFunctionLib.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameName/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

FCodeParseResult UCodeEditingFunctionLib::HandleCodeChange(FString NewCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE, AActor* WorldContextObject)
{
	FCodeParseResult Result;
	TArray<FString> Expressions = GetExpressionsFromCode(NewCodeLine, Result.Errors);
	TArray<FCodeAction> Actions;
	for (FString Exp : Expressions)
	{
		Actions.Add(GetActionFromExpression(Exp, Result.Errors));
	}
	if (Result.Errors.Num() != 0)
	{
		return Result;
	}

	for (auto Action : Actions)
	{
		if (!doAction(Action, Context, WorldContextObject, CE))
		{
			Result.Errors.Add(ECodeSyntaxErrorType::InvalidAction);
			Result.bSuccess = false;
		}
	}

	return Result;
}

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
		UE_LOG(LogTemp, Error, TEXT("Context is not soppurted!"));
		return FString();
	}

}

FCodeAction UCodeEditingFunctionLib::GetActionFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errs)
{
	FCodeAction retAction;
	if (Expression.StartsWith("transform.z"))
	{
		FString ValueStr = Expression.RightChop(FString("transform.z").Len() + 1);
		UE_LOG(LogTemp, Warning, TEXT("Value Str: %s"), *ValueStr);
		float Value = FCString::Atof(*ValueStr);
		if (Value == 0 && FCString::Atof(*(ValueStr.RightChop(1))) != 0)
		{
			Value = FCString::Atof(*ValueStr.RightChop(1));
		}
		retAction.ActedOnField = EActedOnField::TransformZ;
		retAction.ExpressionType = GetExpressionTypeFromExpression(Expression, Errs);
		retAction.Value = Value;
	}
	return retAction;
}

TArray<FString> UCodeEditingFunctionLib::GetExpressionsFromCode(FString Code, TArray<ECodeSyntaxErrorType>& Errs)
{
	Code = Code.Replace(TEXT(" "), TEXT("")); // Remove spaces for easier parsing
	Code = Code.ToLower();
	TArray<FString> Expressions;
	Code.ParseIntoArray(Expressions, TEXT("\n"));
	return Expressions;
}

EExpressionType UCodeEditingFunctionLib::GetExpressionTypeFromExpression(FString Expression, TArray<ECodeSyntaxErrorType>& Errs)
{
		EExpressionType ret = Expression.Find(TEXT("+=")) != INDEX_NONE ? EExpressionType::Addition :
		Expression.Find(TEXT("-=")) != INDEX_NONE ? EExpressionType::Subtraction :
		Expression.Find(TEXT("=")) != INDEX_NONE ? EExpressionType::Set :
		EExpressionType::Invalid;
		if (ret == EExpressionType::Invalid)
		{
			Errs.Add(ECodeSyntaxErrorType::InvalidExpression);
		}
		return ret;
}

bool UCodeEditingFunctionLib::doAction(FCodeAction Action, TSubclassOf<AActor> Context, AActor* WorldContextObject, ECodeEvent CE)
{
	TArray<AActor*, FDefaultAllocator> OutObjects;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, Context, OutObjects);
	if (OutObjects.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Instances found of object!"));
		return true;
	}
	for (auto Actor : OutObjects)
	{
		//player actions:
		if (Context->IsChildOf(ATP_ThirdPersonCharacter::StaticClass()))
		{
			ATP_ThirdPersonCharacter* CastedActor = Cast<ATP_ThirdPersonCharacter>(Actor);
			if (CE == ECodeEvent::OnJump)
			{
				if (Action.ActedOnField == EActedOnField::TransformZ)
				{
					if (Action.ExpressionType == EExpressionType::Set)
					{
						CastedActor->JumpZValue = Action.Value;
						CastedActor->JumpType = EJumpType::Set;
					}
					else if (Action.ExpressionType == EExpressionType::Addition)
					{
						CastedActor->JumpType = EJumpType::Jump;
						CastedActor->GetCharacterMovement()->JumpZVelocity = Action.Value;
						UE_LOG(LogTemp, Warning, TEXT("Set JumpZVelocity for character: %s to %f"), *CastedActor->GetName(), Action.Value)
					}
					else if (Action.ExpressionType == EExpressionType::Subtraction)
					{
						CastedActor->JumpZValue = Action.Value;
						CastedActor->JumpType = EJumpType::Down;
					}
				}
			}
		}
	}
	return true;
}
