// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

class URPGItem;
class URPGSaveGame;

UCLASS()
class RPGZELDA_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URPGGameInstance();

	/** Number of slots for each type of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FPrimaryAssetType, int32> ItemsPerType;

	//인벤토리와 게임 세이브
	/** The slot name used for saving */
	UPROPERTY(BlueprintReadWrite, Category = Save)
		FString SaveSlotName;
	/** The platform-specific user index */
	UPROPERTY(BlueprintReadWrite, Category = Save)
		int32 SaveUserIndex;

	/** Returns the current save game, so it can be used to initialize state. Changes are not written until WriteSaveGame is called */
	UFUNCTION(BlueprintCallable, Category = Save)
		URPGSaveGame* GetCurrentSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
		void WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
		void LoadSaveGame(APlayerController* PlayerController);

protected:
	/** The current save game object */
	UPROPERTY()
		URPGSaveGame* CurrentSaveGame;
};
