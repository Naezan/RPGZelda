// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDec_IsRanged.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTDec_IsRanged : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDec_IsRanged();

protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory) const override;
};
