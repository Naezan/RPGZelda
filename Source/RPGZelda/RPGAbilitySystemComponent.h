// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

class URPGGameplayAbility;

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Constructors and overrides
	URPGAbilitySystemComponent();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, 
		TArray<URPGGameplayAbility*>& ActiveAbilities);

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static URPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, 
		bool LookForComponent = false);
};
