// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolNPCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

APatrolNPCAIController::APatrolNPCAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_SD(
		TEXT("BlackboardData'/Game/AI/BB_NPC.BB_NPC'")
	);
	if (BB_SD.Succeeded())
	{
		BBComp = BB_SD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_SD(
		TEXT("BehaviorTree'/Game/AI/NPCAI/BT_PatrolNPC.BT_PatrolNPC'")
	);
	if (BT_SD.Succeeded())
	{
		BTComp = BT_SD.Object;
	}
}
