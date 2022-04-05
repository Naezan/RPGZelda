// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

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

protected:
	bool bInputMove;

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

	void SetNewDestination(const FVector DestLocation);

	void MoveToMouseCursor();

protected:
	class ALinkCharacter* MainCharacter;

	bool bIsInputLocked;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Player UI")
		class UIngameUI* InGameUI;

	TSubclassOf<class UUserWidget> InGameUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class UDataTable*  PlayerDialogue;

protected:
	UFUNCTION()
		void OnPlayerDialogueCompleted();

public:
	FORCEINLINE UIngameUI* GetUI() { return InGameUI; }
};
