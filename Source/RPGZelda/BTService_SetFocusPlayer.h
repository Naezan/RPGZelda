// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetFocusPlayer.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UBTService_SetFocusPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_SetFocusPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;

		//�Ʒ���尡 Ȱ��ȭ�ɶ� ȣ���
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory) override;
};
