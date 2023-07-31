// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameUI.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UIngameUI::SetMessage(const FString& Text)
{
	if (PlayerDialogTextBlock == nullptr) return;

	PlayerDialogTextBlock->SetText(FText::FromString(Text));
}

void UIngameUI::SetCharacterName(const FString& Text)
{
	if (CharacterNameText == nullptr) return;

	CharacterNameText->SetText(FText::FromString(Text));
}

void UIngameUI::InitializeDialogue(UDataTable* DialogueTable)
{
	//데이터 테이블에서 데이터 초기화
	Dialogue.Empty();

	CurrentState = 0;

	CharacterNameText->SetText(FText::FromString(""));
	PlayerDialogTextBlock->SetText(FText::FromString(""));

	OnResetOptions();

	for (auto it : DialogueTable->GetRowMap())
	{
		FNPCData* Row = (FNPCData*)it.Value;

		Dialogue.Add(Row);
	}

	RowIndex = 0;

	if (Dialogue.Num() > 0)
	{
		CurrentState = 0;

		RowIndex = 0;

		CharacterNameText->
			SetText(FText::FromString(
				Dialogue[RowIndex]->CharacterName.ToString()));

		if (Dialogue[RowIndex]->Messages.Num() > 0)
		{
			MessageIndex = 0;

			OnAnimationShowMessageUI();

			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle, this, &UIngameUI::OnTimerCompleted, 0.5f, false);
		}
	}
}

void UIngameUI::AnimateMessage(const FString& Text)
{
	CurrentState = 1;

	InitialMessage = Text;

	OutputMessage = "";

	iLetter = 0;

	PlayerDialogTextBlock->SetText(FText::FromString(""));

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, this, &UIngameUI::OnAnimationTimerCompleted, DelayBetweenLetters, false);
}

void UIngameUI::OnSelectUpOption()
{
	if (CurrentState != 3) return;

	if ((SelectedOption - 1) >= 0)
	{
		SelectedOption -= 1;
		OnHighLightOption(SelectedOption);
	}
}

void UIngameUI::OnSelectDownOption()
{
	if (CurrentState != 3) return;

	if ((SelectedOption + 1) < Dialogue[RowIndex]->MessageOptions.Num())
	{
		SelectedOption += 1;
		OnHighLightOption(SelectedOption);
	}
}

void UIngameUI::InteractUI()
{
	//skip textmessage
	if (CurrentState == 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		PlayerDialogTextBlock->SetText(FText::FromString(InitialMessage));

		CurrentState = 2;
	}
	else if (CurrentState == 2)
	{
		//대화가 끝난상태라면 대화종료

		//현재문장이 끝난경우 다음문장으로
		if ((MessageIndex + 1) < Dialogue[RowIndex]->Messages.Num())
		{
			MessageIndex += 1;

			AnimateMessage(Dialogue[RowIndex]->Messages[MessageIndex].ToString());
		}
		else
		{
			//끝문장인경우

			CharacterNameText->SetText(FText::FromString("Link"));
			PlayerDialogTextBlock->SetText(FText::FromString(""));

			//답변을 해야하는 문장인 경우
			if (Dialogue[RowIndex]->MessageOptions.Num() > 0)
			{
				OnResetOptions();

				for (int i = 0; i < Dialogue[RowIndex]->MessageOptions.Num(); ++i)
				{
					OnSetOption(i, Dialogue[RowIndex]->MessageOptions[i].OptionText);
				}

				SelectedOption = 0;

				OnHighLightOption(SelectedOption);

				CurrentState = 3; //wait to select answer
			}
			else
			{
				//대화 종료
				CurrentState = 0;
				OnAnimationHideMessageUI();

				OnDialogueCompleted.Broadcast();
			}
		}
	}
	else if (CurrentState == 3)
	{
		//답변도중 스킵을 누를경우
		RowIndex = Dialogue[RowIndex]->MessageOptions[SelectedOption].AnswerIndex;
		OnResetOptions();

		//답변 다음 문장이 있는경우 다음문장으로 이동
		if (RowIndex >= 0 && (RowIndex < Dialogue.Num()))
		{
			PlayerDialogTextBlock->SetText(FText::FromString(""));

			MessageIndex = 0;
			AnimateMessage(Dialogue[RowIndex]->Messages[MessageIndex].ToString());
		}
		else
		{
			//대화종료
			CurrentState = 0;
			OnAnimationHideMessageUI();

			OnDialogueCompleted.Broadcast();
		}
	}
}

void UIngameUI::OnTimerCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	AnimateMessage(Dialogue[RowIndex]->Messages[MessageIndex].ToString());
}

void UIngameUI::OnAnimationTimerCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	OutputMessage.AppendChar(InitialMessage[iLetter]);

	PlayerDialogTextBlock->SetText(FText::FromString(OutputMessage));

	if (SoundCueMessage != nullptr)
	{
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundCueMessage, .5f);
	}

	if ((iLetter + 1) < InitialMessage.Len())
	{
		iLetter = iLetter + 1;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,
			&UIngameUI::OnAnimationTimerCompleted, DelayBetweenLetters, false);
	}
	else
	{
		CurrentState = 2;
	}
}
