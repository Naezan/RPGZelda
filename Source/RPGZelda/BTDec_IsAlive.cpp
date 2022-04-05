// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDec_IsAlive.h"
#include "CharacterBase.h"
#include "MonsterAIController.h"

UBTDec_IsAlive::UBTDec_IsAlive()
{
	NodeName = TEXT("I am Alive");
}

bool UBTDec_IsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) 
		return false;

	ACharacterBase* Monster = Cast<ACharacterBase>(ControllingPawn);
	if (nullptr == Monster)
		return false;

	return Monster->IsAlive();
}
