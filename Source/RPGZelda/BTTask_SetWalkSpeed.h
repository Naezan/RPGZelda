// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetWalkSpeed.generated.h"

/**
 *
 */
UCLASS()
class RPGZELDA_API UBTTask_SetWalkSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetWalkSpeed();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;
};
