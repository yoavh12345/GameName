// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeEditingFunctionLib.h"
#include "GameName/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

void UCodeEditingFunctionLib::HandleCodeChange(FString NewCodeLine, TSubclassOf<AActor> Context, ECodeEvent CE)
{
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
