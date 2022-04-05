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

	//몬스터 찾기
	ACharacterBase* Monster = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
		return EBTNodeResult::Failed;

	//몬스터 공격
	Monster->Attack();
	IsAttacking = true;

	//몬스터 공격 종료
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
		//공격이 종료되면 태스크 종료
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
