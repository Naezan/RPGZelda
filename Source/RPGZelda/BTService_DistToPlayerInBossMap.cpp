// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistToPlayerInBossMap.h"
#include "CharacterBase.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_DistToPlayerInBossMap::UBTService_DistToPlayerInBossMap()
{
	NodeName = TEXT("Distance To Player In Boss Stage");
}

void UBTService_DistToPlayerInBossMap::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//보스
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	//플레이어
	UObject* TargetObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(ABossAIController::TARGET_KEY);

	//플레이어가 있으면
	if (TargetObject)
	{
		AActor* Target = Cast<AActor>(TargetObject);
		if (Target)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
				ABossAIController::TARGET_LENGTH_KEY, ControllingPawn->GetDistanceTo(Target));
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
			ABossAIController::TARGET_LENGTH_KEY, NULL);
	}
}
