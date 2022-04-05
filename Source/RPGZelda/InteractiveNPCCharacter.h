// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacterBase.h"
#include "InteractiveNPCCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class UDataTable* Dialogue;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
		USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
		UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f))
		float CameraBlendTime;

protected:

	UFUNCTION()
	void OnDialogueCompleted();

	bool bIsInteracting = false;
};
