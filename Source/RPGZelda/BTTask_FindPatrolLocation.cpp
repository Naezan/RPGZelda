// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolLocation.h"
#include "NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "NPCCharacterBase.h"

UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("Find Patrol Location");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//컨트롤하는 오브젝트
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	//센터
	UObject* CenterObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(ANPCAIController::TARGET_KEY);
	if (CenterObject == nullptr) return EBTNodeResult::Failed;

	AActor* Center = Cast<AActor>(CenterObject);
	if (Center == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem =
		UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Center->GetActorLocation(), 300.f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->
			SetValueAsVector(ANPCAIController::TARGET_LOCATION_KEY, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
