// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetStrafingLocation.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_GetStrafingLocation::UBTTask_GetStrafingLocation()
{
	NodeName = TEXT("Get Strafing Location");
}

EBTNodeResult::Type UBTTask_GetStrafingLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//컨트롤하는 오브젝트
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn != nullptr)
	{
		Speed = FMath::RandRange(100.f, 200.f);
		StrafeDir = UKismetMathLibrary::SelectInt(-1, 1, FMath::RandBool());
		FVector StrafingLocation =
			ControllingPawn->GetActorLocation() + ControllingPawn->GetActorRightVector() * Speed * StrafeDir;

		OwnerComp.GetBlackboardComponent()->
			SetValueAsVector(ABossAIController::STRAFE_LOCATION_KEY, StrafingLocation);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
