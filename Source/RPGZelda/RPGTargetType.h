// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "RPGTargetType.generated.h"


class ACharacterBase;
class AActor;
struct FGameplayEventData;
/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class RPGZELDA_API URPGTargetType : public UObject
{
	GENERATED_BODY()
	
public:
	// Constructor and overrides
	URPGTargetType() {}

	//게임플레이 이펙트를 적용할 타겟
	UFUNCTION(BlueprintNativeEvent)
		void GetTargets(ACharacterBase* TargetingCharacter,
			AActor* TargetingActor, FGameplayEventData EventData, 
			TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;

	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, 
		AActor* TargetingActor, FGameplayEventData EventData, 
		TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class RPGZELDA_API URPGTargetType_UseOwner : public URPGTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	URPGTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class RPGZELDA_API URPGTargetType_UseEventData : public URPGTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	URPGTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};