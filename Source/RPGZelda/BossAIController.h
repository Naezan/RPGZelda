// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossAIController();
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BTComp;

	UPROPERTY(EditAnywhere)
		class UBlackboardData* BBComp;

public:
	//������ ������ ����
	static const FName CENTER_KEY; //������ ������ �� �ִ� ����
	static const FName CENTER_LOCATION_KEY; //������ ��ġ
	static const FName CENTER_LENGTH_KEY; //���Ϳ��� �Ÿ�

	static const FName TARGET_KEY; //�÷��̾�
	static const FName TARGET_LOCATION_KEY; //�÷��̾� ��ġ
	static const FName TARGET_LENGTH_KEY; //�÷��̾���� �Ÿ�

	static const FName STRAFE_LOCATION_KEY; //�¿� ������ ��ǥ

public:
	AActor* GetSeeingPawn();

	UFUNCTION(BlueprintCallable)
	void RunningBehaviorTree();
};
