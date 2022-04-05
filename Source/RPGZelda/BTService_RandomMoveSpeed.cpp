// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RandomMoveSpeed.h"
#include "CharacterBase.h"
#include "MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_RandomMoveSpeed::UBTService_RandomMoveSpeed()
{

}

void UBTService_RandomMoveSpeed::TickNode(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//¸ó½ºÅÍ
	ACharacter* ControllingCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (nullptr == ControllingCharacter) return;

	ControllingCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
