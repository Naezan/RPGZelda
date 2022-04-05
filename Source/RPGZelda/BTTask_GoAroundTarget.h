// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GoAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTTask_GoAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GoAroundTarget();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
