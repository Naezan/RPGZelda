// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGTypes.h"
#include "Engine/DataTable.h"
#include "RPGSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	//UPROPERTY()
	//	FString PlayerID;

	UPROPERTY()
		float Health;

	UPROPERTY()
		TArray<URPGItem*> GrantedItems;
};

USTRUCT(BlueprintType)
struct FNPCSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which Actor this belongs to */
	UPROPERTY()
		FName NPCName;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY()
		FTransform Transform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
		bool bTalkEnd;
};

USTRUCT(BlueprintType)
struct FChestSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which Actor this belongs to */
	UPROPERTY()
		FName ChestName;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
		bool bIsOpen;

	UPROPERTY()
		bool bMoveComplete;

	UPROPERTY()
		float bAlpha;
};

UCLASS()
class RPGZELDA_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString SaveSlotName;		// Save game data file name

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 SaveIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FPlayerSaveData> SavedPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FNPCSaveData> SavedNPCs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FChestSaveData> SavedChests;
};
