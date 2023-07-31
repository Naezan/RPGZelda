// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGBowItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGBowItem : public URPGItem
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	URPGBowItem()
	{
		ItemType = URPGAssetManager::WeaponItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		TSubclassOf<AActor> WeaponActor;
};
