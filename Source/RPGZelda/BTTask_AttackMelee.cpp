// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackMelee.h"
#include "MonsterAIController.h"
#include "CharacterBase.h"

UBTTask_AttackMelee::UBTTask_AttackMelee()
{
	bNotifyTick = true;
	IsAttacking = false;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_AttackMelee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//���� ã��
	ACharacterBase* Monster = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
		return EBTNodeResult::Failed;

	//���� ����
	Monster->Attack();
	IsAttacking = true;

	//���� ���� ����
	Monster->OnAttackEndDelegate.AddLambda([this]() -> void {
		IsAttacking = false;
	});
	return EBTNodeResult::InProgress;
}

void UBTTask_AttackMelee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		//������ ����Ǹ� �½�ũ ����
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
