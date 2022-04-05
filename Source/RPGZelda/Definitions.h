// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Definitions.generated.h"

USTRUCT(BlueprintType)
struct FMessageOptions
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FText OptionText;
	UPROPERTY(EditAnywhere)
		int32 AnswerIndex;
};

USTRUCT(BlueprintType)
struct FNPCData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName CharacterName;
	UPROPERTY(EditAnywhere)
		TArray<FText> Messages;
	UPROPERTY(EditAnywhere)
		TArray<FMessageOptions> MessageOptions;
};

/**
 * 
 */
class RPGZELDA_API Definitions
{
public:
	Definitions();
	~Definitions();
};
