// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDec_StopMoving.h"
#include "CharacterBase.h"
#include "AIController.h"

UBTDec_StopMoving::UBTDec_StopMoving()
{
	NodeName = TEXT("Get Stop Moving");
}

bool UBTDec_StopMoving::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	ACharacterBase* MyCharacter = Cast<ACharacterBase>(ControllingPawn);
	if (nullptr == MyCharacter)
		return false;

	return MyCharacter->GetStopMoving();
}
