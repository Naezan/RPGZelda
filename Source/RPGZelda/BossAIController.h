// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossAIController();
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BTComp;

	UPROPERTY(EditAnywhere)
		class UBlackboardData* BBComp;

public:
	//보스가 움직일 범위
	static const FName CENTER_KEY; //보스가 움직일 수 있는 범위
	static const FName CENTER_LOCATION_KEY; //센터의 위치
	static const FName CENTER_LENGTH_KEY; //센터와의 거리

	static const FName TARGET_KEY; //플레이어
	static const FName TARGET_LOCATION_KEY; //플레이어 위치
	static const FName TARGET_LENGTH_KEY; //플레이어와의 거리

	static const FName STRAFE_LOCATION_KEY; //좌우 무빙할 좌표

public:
	AActor* GetSeeingPawn();

	UFUNCTION(BlueprintCallable)
	void RunningBehaviorTree();
};
