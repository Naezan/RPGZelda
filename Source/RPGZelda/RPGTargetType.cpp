// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTargetType.h"
#include "CharacterBase.h"
#include "Abilities/GameplayAbility.h"

void URPGTargetType::GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

//�θ� ����� Ÿ��
void URPGTargetType_UseOwner::GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

//�̺�Ʈ �����Ϳ��� �̾Ƴ� Ÿ��(�����÷��� ����Ʈ�� ������)
void URPGTargetType_UseEventData::GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}