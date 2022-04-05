// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveNPCCharacter.h"
#include "RPGPlayerController.h"
#include "IngameUI.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

	//ī�޶� ������Ʈ ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("NPCCamera"));
	CameraComp->SetupAttachment(CameraSpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void AInteractiveNPCCharacter::Interact()
{
	Super::Interact();

	if (bIsInteracting) return;

	if ((CurrentPlayerController != nullptr) && (Dialogue != nullptr))
	{
		bIsInteracting = true;
		CurrentPlayerController->SetCinematicMode(true, true, true);

		CurrentPlayerController->GetUI()->InitializeDialogue(Dialogue);

		//����
		if (CurrentCharacter != nullptr)
		{
			CurrentPlayerController->SetViewTargetWithBlend(
				CameraComp->GetOwner(), CameraBlendTime, 
				EViewTargetBlendFunction::VTBlend_Linear);
		}

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

		CurrentPlayerController->GetUI()->
			OnDialogueCompleted.RemoveDynamic(this, &AInteractiveNPCCharacter::OnDialogueCompleted);
	}

	bIsInteracting = false;
}
