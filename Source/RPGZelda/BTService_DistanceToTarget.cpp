// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistanceToTarget.h"
#include "CharacterBase.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_DistanceToTarget::UBTService_DistanceToTarget()
{
	NodeName = TEXT("Distance To Target");
}

void UBTService_DistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//����
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;
	//UE_LOG(LogTemp, Warning, TEXT("MonsterPawnReady"));

	//�÷��̾�
	UObject* TargetObject =
		OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(AMonsterAIController::TARGET_KEY);

	//�÷��̾ ������
	if (TargetObject)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PlayerReady"));
		AActor* Target = Cast<AActor>(TargetObject);
		if (Target)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
				AMonsterAIController::TARGET_LENGTH_KEY, ControllingPawn->GetDistanceTo(Target));
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
			AMonsterAIController::TARGET_LENGTH_KEY, NULL);
	}
}
