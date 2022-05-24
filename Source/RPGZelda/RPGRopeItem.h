// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGRopeItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGRopeItem : public URPGItem
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	URPGRopeItem()
	{
		ItemType = URPGAssetManager::RopeItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tool)
		TSubclassOf<AActor> WeaponActor;
};
