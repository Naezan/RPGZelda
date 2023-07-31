// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoAroundPlayerInBossStage.h"
#include "CharacterBase.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_GoAroundPlayerInBossStage::UBTTask_GoAroundPlayerInBossStage()
{
	NodeName = TEXT("Find New Location Around Player");

	Radius = 500.f;
}

EBTNodeResult::Type UBTTask_GoAroundPlayerInBossStage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	//플레이어
	UObject* TargetObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(ABossAIController::TARGET_KEY);
	if (TargetObject == nullptr) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(TargetObject);
	if (Target == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem =
		UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation NextStrafing;
	if (NavSystem->GetRandomPointInNavigableRadius(Target->GetActorLocation(), Radius, NextStrafing))
	{
		OwnerComp.GetBlackboardComponent()->
			SetValueAsVector(ABossAIController::TARGET_LOCATION_KEY, NextStrafing.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
