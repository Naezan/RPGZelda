// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABossAIController::CENTER_KEY = TEXT("Center");
const FName ABossAIController::CENTER_LOCATION_KEY = TEXT("CenterLocation");
const FName ABossAIController::CENTER_LENGTH_KEY = TEXT("DistToCenter");

const FName ABossAIController::TARGET_KEY = TEXT("Target");
const FName ABossAIController::TARGET_LOCATION_KEY = TEXT("TargetLocation");
const FName ABossAIController::TARGET_LENGTH_KEY = TEXT("DistToTarget");

const FName ABossAIController::STRAFE_LOCATION_KEY = TEXT("StrafingLocation");

ABossAIController::ABossAIController()
{
}

void ABossAIController::PostInitializeComponents()
{
	//AI가 플레이어의 상태를 원하는지 여부
	bWantsPlayerState = true;
	Super::PostInitializeComponents();
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//컨트롤러가 소유하고있고 블랙보드를 사용중이라면 행동트리 실행
	//이후에 보스가 완전히 생성되고 난 뒤에 사용할 수 있어야 한다
	if (UseBlackboard(BBComp, Blackboard))
	{
		RunBehaviorTree(BTComp);
	}
}

AActor* ABossAIController::GetSeeingPawn()
{
	//플레이어 찾기
	UObject* object = GetBlackboardComponent()->GetValueAsObject(TARGET_KEY);

	return object ? Cast<AActor>(object) : nullptr;
}

void ABossAIController::RunningBehaviorTree()
{

}
