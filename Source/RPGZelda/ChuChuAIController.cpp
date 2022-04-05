// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuChuAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AChuChuAIController::AChuChuAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_SD(
		TEXT("BlackboardData'/Game/AI/BB_Base.BB_Base'")
	);
	if (BB_SD.Succeeded())
	{
		BBComp = BB_SD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_SD(
		TEXT("BehaviorTree'/Game/AI/ChuChuAI/BT_ChuChu.BT_ChuChu'")
	);
	if (BT_SD.Succeeded())
	{
		BTComp = BT_SD.Object;
	}
}