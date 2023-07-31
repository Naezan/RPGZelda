// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindNPCCenter.h"
#include "InteractiveNPCCharacter.h"
#include "PatrolNPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "PatrolPathActor.h"

UBTService_FindNPCCenter::UBTService_FindNPCCenter()
{
	NodeName = TEXT("Find NPC Center");
}

void UBTService_FindNPCCenter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//��Ʈ���ϴ� ������Ʈ
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	//�ݰ� 6���� ���� ����Ʈ ã��
	UWorld* World = ControllingPawn->GetWorld();
	FVector CenterLocation = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.f;
	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		CenterLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	//ĳ���͸� ã���� ���������� �˻�
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			APatrolPathActor* CenterActor = Cast<APatrolPathActor>(OverlapResult.GetActor());
			if (CenterActor != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->
					SetValueAsObject(ANPCAIController::TARGET_KEY, CenterActor);

				DrawDebugSphere(World, CenterActor->GetActorLocation(), DetectRadius, 16, FColor::Green, false, 1.0f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()
			->SetValueAsObject(ANPCAIController::TARGET_KEY, nullptr);
	}
}
