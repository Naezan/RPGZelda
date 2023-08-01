// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayerInBossStage.h"
#include "CharacterBase.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_FindPlayerInBossStage::UBTService_FindPlayerInBossStage()
{
	NodeName = TEXT("Find Player In Boss Stage");
}

void UBTService_FindPlayerInBossStage::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 2000.f;

	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	//캐릭터를 찾으면 플레이어인지 검사
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ACharacterBase* Player = Cast<ACharacterBase>(OverlapResult.GetActor());
			if (Player && Player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->
					SetValueAsObject(ABossAIController::TARGET_KEY, Player);

				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, Player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), Player->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()
			->SetValueAsObject(ABossAIController::TARGET_KEY, nullptr);
	}

	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
