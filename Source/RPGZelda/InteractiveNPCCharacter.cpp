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
	//스프링암 컴포넌트 생성
	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 150.f),
		FRotator(-10.f, 0.f, 0.f));
	CameraSpringArmComp->TargetArmLength = 150.f;
	CameraSpringArmComp->bEnableCameraLag = true;
	CameraSpringArmComp->CameraLagSpeed = 5.f;
	CameraSpringArmComp->SetUsingAbsoluteRotation(true);//스프링암의 좌표가 월드의 변화에 따라감
	CameraSpringArmComp->SetupAttachment(GetMesh());
	CameraSpringArmComp->bDoCollisionTest = false;

	//카메라 컴포넌트 생성
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

		//퀘스트가 끝났다면 기존의 텍스트를 퀘스트텍스트로 바꿔준다
		if (bIsQuestDone && QuestDialogue != nullptr)
		{
			Dialogue = QuestDialogue;

			//게임세이브
			URPGGameInstance* GameInstanceRef = Cast<URPGGameInstance>(
				UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstanceRef->WriteSaveGame();
		}
		CurrentPlayerController->GetUI()->InitializeDialogue(Dialogue);
		UE_LOG(LogTemp, Warning, TEXT("InitDialogue"));

		//줌인
		if (CurrentCharacter != nullptr)
		{
			CurrentPlayerController->SetViewTargetWithBlend(
				CameraComp->GetOwner(), CameraBlendTime,
				EViewTargetBlendFunction::VTBlend_Linear);
		}

		//애니메이션 있으면 호출
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

		//줌아웃
		if (CurrentCharacter != nullptr)
		{
			CurrentPlayerController->SetViewTargetWithBlend(
				CurrentPlayerController->GetPawn(), CameraBlendTime,
				EViewTargetBlendFunction::VTBlend_Linear);
		}

		//애니메이션 있으면 호출
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