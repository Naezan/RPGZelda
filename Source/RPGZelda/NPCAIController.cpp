// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"

const FName ANPCAIController::TARGET_KEY = TEXT("Center");
const FName ANPCAIController::TARGET_LOCATION_KEY = TEXT("MoveToLocation");
const FName ANPCAIController::TARGET_LENGTH_KEY = TEXT("DistToCenter");

ANPCAIController::ANPCAIController()
{

}

void ANPCAIController::PostInitializeComponents()
{
	//AI가 플레이어의 상태를 원하는지 여부
	bWantsPlayerState = true;
	Super::PostInitializeComponents();
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//컨트롤러가 소유하고있고 블랙보드를 사용중이라면 행동트리 실행
	if (UseBlackboard(BBComp, Blackboard))
	{
		RunBehaviorTree(BTComp);
	}
}
