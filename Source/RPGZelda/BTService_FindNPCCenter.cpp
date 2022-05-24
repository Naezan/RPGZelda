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

	//컨트롤하는 오브젝트
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	//반경 6미터 경유 포인트 찾기
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

	//캐릭터를 찾으면 경유지인지 검사
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
