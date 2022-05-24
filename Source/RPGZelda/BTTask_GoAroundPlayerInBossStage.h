// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GoAroundPlayerInBossStage.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTTask_GoAroundPlayerInBossStage : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GoAroundPlayerInBossStage();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Radius;
};
