// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDec_IsInteracting.h"
#include "InteractiveNPCCharacter.h"
#include "NPCAIController.h"

UBTDec_IsInteracting::UBTDec_IsInteracting()
{
	NodeName = TEXT("NPC Is Not Interacting");
}

bool UBTDec_IsInteracting::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//bIsInteracting;
	//NPC Æù
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	AInteractiveNPCCharacter* NPCCharacter = Cast<AInteractiveNPCCharacter>(ControllingPawn);
	if (nullptr == NPCCharacter)
		return false;

	return !NPCCharacter->GetInteracting();
}
