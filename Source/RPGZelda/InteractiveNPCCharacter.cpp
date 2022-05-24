// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveNPCCharacter.h"
#include "RPGPlayerController.h"
#include "IngameUI.h"
#include "RPGGameInstance.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

AInteractiveNPCCharacter::AInteractiveNPCCharacter()
{
	//�������� ������Ʈ ����
	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 150.f),
		FRotator(-10.f, 0.f, 0.f));
	CameraSpringArmComp->TargetArmLength = 150.f;
	CameraSpringArmComp->bEnableCameraLag = true;
	CameraSpringArmComp->CameraLagSpeed = 5.f;
	CameraSpringArmComp->SetUsingAbsoluteRotation(true);//���������� ��ǥ�� ������ ��ȭ�� ����
	CameraSpringArmComp->SetupAttachment(GetMesh());
	CameraSpringArmComp->bDoCollisionTest = false;

	//ī�޶� ������Ʈ ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("NPCCamera"));
	CameraComp->SetupAttachment(CameraSpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	bIsQuestDone = false;
}

void AInteractiveNPCCharacter::Interact()
{
	Super::Interact();

	if (bIsInteracting) return;

	if ((CurrentPlayerController != nullptr) && (Dialogue != nullptr))
	{
		bIsInteracting = true;
		CurrentPlayerController->SetCinematicMode(true, true, true);

		//����Ʈ�� �����ٸ� ������ �ؽ�Ʈ�� ����Ʈ�ؽ�Ʈ�� �ٲ��ش�
		if (bIsQuestDone && QuestDialogue != nullptr)
		{
			Dialogue = QuestDialogue;

			//���Ӽ��̺�
			URPGGameInstance* GameInstanceRef = Cast<URPGGameInstance>(
				UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstanceRef->WriteSaveGame();
		}
		CurrentPlayerController->GetUI()->InitializeDialogue(Dialogue);
		UE_LOG(LogTemp, Warning, TEXT("InitDialogue"));

		//����
		if (CurrentCharacter != nullptr)
		{
			CurrentPlayerController->SetViewTargetWithBlend(
				CameraComp->GetOwner(), CameraBlendTime,
				EViewTargetBlendFunction::VTBlend_Linear);
		}

		//�ִϸ��̼� ������ ȣ��
		SetInteractBeginAnimation();

		CurrentPlayerController->ShowHUD(false);

		CurrentPlayerController->GetUI()->
			OnDialogueCompleted.AddDynamic(this, &AInteractiveNPCCharacter::OnDialogueCompleted);
	}
}

void AInteractiveNPCCharacter::OnDialogueCompleted()
{
	if (CurrentPlayerController != nullptr)
	{
		CurrentPlayerController->SetCinematicMode(false, true, true);

		//�ܾƿ�
		if (CurrentCharacter != nullptr)
		{
			CurrentPlayerController->SetViewTargetWithBlend(
				CurrentPlayerController->GetPawn(), CameraBlendTime,
				EViewTargetBlendFunction::VTBlend_Linear);
		}

		//�ִϸ��̼� ������ ȣ��
		SetInteractEndAnimation();

		CurrentPlayerController->ShowHUD(true);

		CurrentPlayerController->GetUI()->
			OnDialogueCompleted.RemoveDynamic(this, &AInteractiveNPCCharacter::OnDialogueCompleted);
	}

	bIsInteracting = false;
}

void AInteractiveNPCCharacter::SetIsQuestDone(bool QuestState)
{
	bIsQuestDone = QuestState;
}