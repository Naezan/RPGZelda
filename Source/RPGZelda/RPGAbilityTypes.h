// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "RPGAbilityTypes.generated.h"

class URPGAbilitySystemComponent;
class UGameplayEffect;
class URPGTargetType;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainer
{
	GENERATED_BODY()

public:
	FRPGGameplayEffectContainer() {}

	/** Sets the way that targeting happens */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		TSubclassOf<URPGTargetType> TargetType;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

/** A "processed" version of RPGGameplayEffectContainer that can be passed around and eventually applied */
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FRPGGameplayEffectContainerSpec() {}

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
		TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const;

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const;

	/** Adds new targets to target data */
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};
