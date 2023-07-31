// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDec_StopMoving.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTDec_StopMoving : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDec_StopMoving();

protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;
};
