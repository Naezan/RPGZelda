// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetFocusPlayer.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetFocusPlayer::UBTTask_SetFocusPlayer()
{
	NodeName = TEXT("Set Focus Player In Boss Stage");
}

EBTNodeResult::Type UBTTask_SetFocusPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* OwnerController =  OwnerComp.GetAIOwner();
	if (nullptr == OwnerController) return EBTNodeResult::Failed;

	//플레이어
	UObject* TargetObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(ABossAIController::TARGET_KEY);
	if (TargetObject == nullptr) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(TargetObject);
	if (Target != nullptr)
	{
		OwnerController->SetFocus(Target, EAIFocusPriority::Gameplay);
		UE_LOG(LogTemp, Warning, TEXT("SucceedTarget"));
		return EBTNodeResult::Succeeded;
	}
	else
	{
		OwnerController->SetFocus(nullptr);
		return EBTNodeResult::Failed;
	}



	return EBTNodeResult::Failed;
}
