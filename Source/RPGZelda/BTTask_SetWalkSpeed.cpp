// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetWalkSpeed.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"

UBTTask_SetWalkSpeed::UBTTask_SetWalkSpeed()
{
	NodeName = TEXT("Set Speed");
}

EBTNodeResult::Type UBTTask_SetWalkSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	ACharacter* ControllingCharacter = Cast<ACharacter>(ControllingPawn);
	if (nullptr == ControllingCharacter) return EBTNodeResult::Failed;

	UCharacterMovementComponent* MovementComp = ControllingCharacter->GetCharacterMovement();
	if (nullptr != MovementComp)
	{
		Speed = FMath::RandRange(100.f, 300.f);
		MovementComp->MaxWalkSpeed = Speed;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
