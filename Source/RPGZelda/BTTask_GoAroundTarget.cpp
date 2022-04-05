// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoAroundTarget.h"
#include "CharacterBase.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_GoAroundTarget::UBTTask_GoAroundTarget()
{
	NodeName = TEXT("Find New Location Around Target");
}

EBTNodeResult::Type UBTTask_GoAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	//플레이어
	UObject* TargetObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(AMonsterAIController::TARGET_KEY);
	if (TargetObject == nullptr) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(TargetObject);
	if (Target == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = 
		UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if(NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Target->GetActorLocation(), 300.f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->
			SetValueAsVector(AMonsterAIController::TARGET_LOCATION_KEY, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}