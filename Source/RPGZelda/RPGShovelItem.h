// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGShovelItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGShovelItem : public URPGItem
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	URPGShovelItem()
	{
		ItemType = URPGAssetManager::ShovelItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tool)
		TSubclassOf<AActor> WeaponActor;
};
