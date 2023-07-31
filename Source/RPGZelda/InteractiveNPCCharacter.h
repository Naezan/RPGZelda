// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacterBase.h"
#include "InteractiveNPCCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDataTable;

/**
 *
 */
UCLASS()
class RPGZELDA_API AInteractiveNPCCharacter : public ANPCCharacterBase
{
	GENERATED_BODY()

public:
	AInteractiveNPCCharacter();

public:
	void Interact() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Animation")
		void SetInteractBeginAnimation();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Animation")
		void SetInteractEndAnimation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TArray<UDataTable*> DialogueArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UDataTable* Dialogue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UDataTable* QuestDialogue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		bool bIsQuestDone;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
		USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
		UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f))
		float CameraBlendTime;

protected:

	UFUNCTION()
		void OnDialogueCompleted();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		bool bIsInteracting = false;

public:
	bool GetInteracting() { return bIsInteracting; }

	bool GetIsQuestDone() { return bIsQuestDone; }
	void SetIsQuestDone(bool QuestState);
};
