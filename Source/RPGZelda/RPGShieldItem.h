// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGShieldItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGShieldItem : public URPGItem
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	URPGShieldItem()
	{
		ItemType = URPGAssetManager::ShieldItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		TSubclassOf<AActor> WeaponActor;
};
