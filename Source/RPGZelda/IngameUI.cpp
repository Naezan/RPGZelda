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
	//������ ���̺��� ������ �ʱ�ȭ
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
		//��ȭ�� �������¶�� ��ȭ����

		//���繮���� ������� ������������
		if ((MessageIndex + 1) < Dialogue[RowIndex]->Messages.Num())
		{
			MessageIndex += 1;

			AnimateMessage(Dialogue[RowIndex]->Messages[MessageIndex].ToString());
		}
		else
		{
			//�������ΰ��

			CharacterNameText->SetText(FText::FromString("Link"));
			PlayerDialogTextBlock->SetText(FText::FromString(""));

			//�亯�� �ؾ��ϴ� ������ ���
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
				//��ȭ ����
				CurrentState = 0;
				OnAnimationHideMessageUI();

				OnDialogueCompleted.Broadcast();
			}
		}
	}
	else if (CurrentState == 3)
	{
		//�亯���� ��ŵ�� �������
		RowIndex = Dialogue[RowIndex]->MessageOptions[SelectedOption].AnswerIndex;
		OnResetOptions();

		//�亯 ���� ������ �ִ°�� ������������ �̵�
		if (RowIndex >= 0 && (RowIndex < Dialogue.Num()))
		{
			PlayerDialogTextBlock->SetText(FText::FromString(""));

			MessageIndex = 0;
			AnimateMessage(Dialogue[RowIndex]->Messages[MessageIndex].ToString());
		}
		else
		{
			//��ȭ����
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
