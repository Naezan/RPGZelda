// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDec_IsRanged.h"
#include "CharacterBase.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDec_IsRanged::UBTDec_IsRanged()
{
	NodeName = TEXT("Deteched");
}

bool UBTDec_IsRanged::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	float Dist = OwnerComp.GetBlackboardComponent()->
		GetValueAsFloat(AMonsterAIController::TARGET_LENGTH_KEY);
	//UE_LOG(LogTemp, Warning, TEXT("Dist : %f"), Dist);
	if (NULL == Dist)
		return false;

	bResult = (Dist >= 400.f && Dist <= 1000.f);
	//UE_LOG(LogTemp, Warning, TEXT("can Attack: %s"), bResult ? TEXT("true") : TEXT("false"));

	return bResult;
}
