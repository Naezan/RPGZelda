// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGBombItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGBombItem : public URPGItem
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	URPGBombItem()
	{
		ItemType = URPGAssetManager::BombItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tool)
		TSubclassOf<AActor> WeaponActor;
};
