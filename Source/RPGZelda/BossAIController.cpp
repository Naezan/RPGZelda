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
	//AI�� �÷��̾��� ���¸� ���ϴ��� ����
	bWantsPlayerState = true;
	Super::PostInitializeComponents();
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//��Ʈ�ѷ��� �����ϰ��ְ� �����带 ������̶�� �ൿƮ�� ����
	//���Ŀ� ������ ������ �����ǰ� �� �ڿ� ����� �� �־�� �Ѵ�
	if (UseBlackboard(BBComp, Blackboard))
	{
		RunBehaviorTree(BTComp);
	}
}

AActor* ABossAIController::GetSeeingPawn()
{
	//�÷��̾� ã��
	UObject* object = GetBlackboardComponent()->GetValueAsObject(TARGET_KEY);

	return object ? Cast<AActor>(object) : nullptr;
}

void ABossAIController::RunningBehaviorTree()
{

}
