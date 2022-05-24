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
	//AI�� �÷��̾��� ���¸� ���ϴ��� ����
	bWantsPlayerState = true;
	Super::PostInitializeComponents();
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//��Ʈ�ѷ��� �����ϰ��ְ� �����带 ������̶�� �ൿƮ�� ����
	if (UseBlackboard(BBComp, Blackboard))
	{
		RunBehaviorTree(BTComp);
	}
}
