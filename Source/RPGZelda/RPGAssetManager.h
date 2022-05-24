// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RPGAssetManager.generated.h"

class URPGItem;

/**
 * 
 */
UCLASS()
class RPGZELDA_API URPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	URPGAssetManager() {}

	virtual void StartInitialLoading() override;

	/** Static types for items */
	static const FPrimaryAssetType	BombItemType;
	static const FPrimaryAssetType	RopeItemType;
	static const FPrimaryAssetType	ShieldItemType;
	static const FPrimaryAssetType	ShovelItemType;
	static const FPrimaryAssetType	SkillItemType;
	static const FPrimaryAssetType	WeaponItemType;

	/** Returns the current AssetManager object */
	static URPGAssetManager& Get();

	URPGItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
