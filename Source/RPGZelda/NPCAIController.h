// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPCAIController();
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY()
		class UBehaviorTree* BTComp;

	UPROPERTY()
		class UBlackboardData* BBComp;

public:
	//대상, 위치, 거리
	static const FName TARGET_KEY;
	static const FName TARGET_LOCATION_KEY;
	static const FName TARGET_LENGTH_KEY;
};
