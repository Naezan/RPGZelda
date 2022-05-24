// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Definitions.h"
#include "IngameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueCompeletedDelegate);

/**
 * 
 */
UCLASS()
class RPGZELDA_API UIngameUI : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerDialogTextBlock;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharacterNameText;

	UPROPERTY(EditAnywhere, Category = "UI Message Settings")
		float DelayBetweenLetters = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Message Settings")
		class USoundBase* SoundCueMessage;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnAnimationShowMessageUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnAnimationHideMessageUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnShowMessage();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnHideMessage();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnResetOptions();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnHighLightOption(int32 Option);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Events")
		void OnSetOption(int32 Option, const FText& OptionText);

public:
	void SetMessage(const FString& Text);

	void SetCharacterName(const FString& Text);

	void InitializeDialogue(class UDataTable* DialogueTable);

	void AnimateMessage(const FString& Text);

	void OnSelectUpOption();

	void OnSelectDownOption();

	void InteractUI();

private:
	FTimerHandle TimerHandle;

	UFUNCTION()
		void OnTimerCompleted();

	UFUNCTION()
		void OnAnimationTimerCompleted();

	FString InitialMessage;

	FString OutputMessage;

	int32 iLetter;

private:

	int32 CurrentState = 0;
	
	int32 SelectedOption;

	TArray<FNPCData*> Dialogue;

	int32 MessageIndex;

	int32 RowIndex;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FDialogueCompeletedDelegate OnDialogueCompleted;
};
