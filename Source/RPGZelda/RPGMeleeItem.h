// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGMeleeItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGMeleeItem : public URPGItem
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	URPGMeleeItem()
	{
		ItemType = URPGAssetManager::WeaponItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		TSubclassOf<AActor> WeaponActor;
};
