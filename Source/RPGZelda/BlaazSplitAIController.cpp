// Fill out your copyright notice in the Description page of Project Settings.


#include "BlaazSplitAIController.h"

ABlaazSplitAIController::ABlaazSplitAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_SD(
		TEXT("BlackboardData'/Game/AI/BB_SplitBoss.BB_SplitBoss'")
	);
	if (BB_SD.Succeeded())
	{
		BBComp = BB_SD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_SD(
		TEXT("BehaviorTree'/Game/AI/BlaazSplitAI/BT_BlaazSplit.BT_BlaazSplit'")
	);
	if (BT_SD.Succeeded())
	{
		BTComp = BT_SD.Object;
	}
}