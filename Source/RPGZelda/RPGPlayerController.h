// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGTypes.h"
#include "RPGPlayerController.generated.h"

class ALinkCharacter;
class URPGSaveGame;
/**
 *
 */
UCLASS()
class RPGZELDA_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARPGPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;

protected:
	bool bInputMove;

	bool bIsAttackButtonDown = false;

	UFUNCTION()
		void StartInputMove();
	UFUNCTION()
		void StopInputMove();

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void Interact();

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void OnKeyUp();

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void OnKeyDown();

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void OnLeftChangeItem();
	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void OnRightChangeItem();

	void SetNewDestination(const FVector DestLocation);

	void MoveToMouseCursor();

protected:
	UFUNCTION()
		void PlayerMoveForward(float Value);
	UFUNCTION()
		void PlayerMoveRight(float Value);
	UFUNCTION()
		void PlayerZooming(float Value);
	UFUNCTION()
		void PlayerStartJump();
	UFUNCTION()
		void PlayerStopJump();
	UFUNCTION()
		void PlayerStartRun();
	UFUNCTION()
		void PlayerStopRun();
	UFUNCTION()
		void BeginPlayerAttack();
	UFUNCTION()
		void KeepPlayerAttack();
	UFUNCTION()
		void EndPlayerAttack();

public:
	UFUNCTION()
		void PlayerAttackEnd();

protected:
	ALinkCharacter* MainCharacter;

	bool bIsInputLocked;

public:
	ALinkCharacter* GetPlayerCharacter();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Player UI")
		class UIngameUI* InGameUI;

	TSubclassOf<class UUserWidget> InGameUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		class UDataTable* PlayerDialogue;

public:
	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FRPGItemInfo, URPGItem*> GrantedItems;

protected:
	UFUNCTION()
		void OnPlayerDialogueCompleted();

public:
	UFUNCTION(BlueprintCallable, Category = "Player UI")
		class UIngameUI* GetUI() { return InGameUI; }

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void Disable();

	UFUNCTION(BlueprintCallable, Category = "Inputs")
		void Enable();

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool LoadRPGItems();

	/** Returns all slotted items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetGrantedItems(TArray<URPGItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool SetGrantedItem(URPGItem* Item);

	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
		void GrantedItemChanged();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Inventory)
		void ShowHUD(bool Visible);

	UFUNCTION(BlueprintImplementableEvent)
		void SavePlayerData(URPGSaveGame* SaveObject);

	UFUNCTION(BlueprintImplementableEvent)
		void LoadPlayerData(URPGSaveGame* SaveObject);

	UFUNCTION(BlueprintImplementableEvent)
		void InitSaveData();
};
