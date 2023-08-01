// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindCenterPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossAIController.h"
#include "DrawDebugHelpers.h"

#include "CenterActor.h"

UBTService_FindCenterPoint::UBTService_FindCenterPoint()
{
	NodeName = TEXT("Find Center Point");
}

void UBTService_FindCenterPoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//��Ʈ���ϴ� ������Ʈ
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	//���� ����Ʈ ã��
	UWorld* World = ControllingPawn->GetWorld();
	FVector CenterLocation = ControllingPawn->GetActorLocation();
	float DetectRadius = 2000.f;
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

	//���͸� ã������ �������� �˻�
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ACenterActor* CenterActor = Cast<ACenterActor>(OverlapResult.GetActor());
			if (CenterActor != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->
					SetValueAsObject(ABossAIController::CENTER_KEY, CenterActor);

				//DrawDebugSphere(World, CenterActor->GetActorLocation(), DetectRadius, 16, FColor::Green, false, 1.0f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()
			->SetValueAsObject(ABossAIController::CENTER_KEY, nullptr);
	}
}
