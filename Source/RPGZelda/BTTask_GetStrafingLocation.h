// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetStrafingLocation.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTTask_GetStrafingLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GetStrafingLocation();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 StrafeDir;
};
