// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAssetManager.h"
#include "RPGItem.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType URPGAssetManager::BombItemType = TEXT("Bomb");
const FPrimaryAssetType URPGAssetManager::RopeItemType = TEXT("Rope");
const FPrimaryAssetType URPGAssetManager::ShieldItemType = TEXT("Shield");
const FPrimaryAssetType URPGAssetManager::ShovelItemType = TEXT("Shovel");
const FPrimaryAssetType URPGAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType URPGAssetManager::WeaponItemType = TEXT("Weapon");

URPGAssetManager& URPGAssetManager::Get()
{
	URPGAssetManager* This = Cast<URPGAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<URPGAssetManager>(); // never calls this
	}
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

URPGItem* URPGAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	URPGItem* LoadedItem = Cast<URPGItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}