// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindCenterPoint.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTService_FindCenterPoint : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindCenterPoint();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;
};
