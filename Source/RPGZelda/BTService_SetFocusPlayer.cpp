// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetFocusPlayer.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SetFocusPlayer::UBTService_SetFocusPlayer()
{
	NodeName = TEXT("Set Focus Player");

	bNotifyBecomeRelevant = true;
}

void UBTService_SetFocusPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}

void UBTService_SetFocusPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (nullptr == OwnerController) return;

	//플레이어
	UObject* TargetObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(ABossAIController::TARGET_KEY);
	if (TargetObject == nullptr) return;

	AActor* Target = Cast<AActor>(TargetObject);
	if (Target != nullptr)
	{
		OwnerController->SetFocus(Target, EAIFocusPriority::Gameplay);
		UE_LOG(LogTemp, Warning, TEXT("SucceedTarget"));
	}
	else
	{
		OwnerController->SetFocus(nullptr);
	}

	return;
}
