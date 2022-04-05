// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "IngameUI.h"
#include "Blueprint/UserWidget.h"
#include "LinkCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "InteractiveNPCCharacter.h"

ARPGPlayerController::ARPGPlayerController()
{
	bShowMouseCursor = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> 
		InGameMenuBPClass(TEXT("/Game/UI/WBP_InGameUI"));

	if (InGameMenuBPClass.Class != nullptr)
	{
		InGameUIClass = InGameMenuBPClass.Class;
	}
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	APawn* const pawn = GetPawn();

	if (pawn != nullptr)
	{
		MainCharacter = Cast<ALinkCharacter>(pawn);
	}

	if (InGameUIClass != nullptr)
	{
		InGameUI = CreateWidget<UIngameUI>(this, InGameUIClass);
	}

	if (InGameUI != nullptr)
	{
		InGameUI->AddToViewport();

		//처음엔 안보이도록
		InGameUI->OnAnimationHideMessageUI();

		//InGameUI->AnimateMessage("Link", "This is Animated Message");

		//test
		//InGameUI->SetMessage("Hello World!");
		//InGameUI->SetCharacterName("Link");
		//InGameUI->OnAnimationShowMessageUI();

		if (PlayerDialogue != nullptr)
		{
			//SetCinematicMode(true, true, true);
			//InGameUI->InitializeDialogue(PlayerDialogue);

			//InGameUI->OnDialogueCompleted.AddDynamic(
			//	this, &ARPGPlayerController::OnPlayerDialogueCompleted);
		}
	}
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bInputMove)
	{
		MoveToMouseCursor();
	}
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InputMove", IE_Pressed, this, &ARPGPlayerController::StartInputMove);
	InputComponent->BindAction("InputMove", IE_Released, this, &ARPGPlayerController::StopInputMove);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ARPGPlayerController::Interact);

	InputComponent->BindAction("KeyUp", IE_Pressed, this, &ARPGPlayerController::OnKeyUp).bConsumeInput = false;
	InputComponent->BindAction("KeyDown", IE_Pressed, this, &ARPGPlayerController::OnKeyDown).bConsumeInput = false;

}

void ARPGPlayerController::StartInputMove()
{
	bInputMove = true;
}

void ARPGPlayerController::StopInputMove()
{
	bInputMove = false;
}

void ARPGPlayerController::Interact()
{
	if ((MainCharacter != nullptr) && (MainCharacter->GetInteractive() != nullptr))
	{
		MainCharacter->GetInteractive()->Interact();
	}

	if (InGameUI != nullptr)
	{
		InGameUI->Interact();
	}
}

void ARPGPlayerController::OnKeyUp()
{
	if (InGameUI != nullptr)
	{
		InGameUI->OnSelectUpOption();
	}
}

void ARPGPlayerController::OnKeyDown()
{
	if (InGameUI != nullptr)
	{
		InGameUI->OnSelectDownOption();
	}
}

void ARPGPlayerController::SetNewDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		if (Distance > 120.f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ARPGPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.ImpactPoint);
	}
}

void ARPGPlayerController::OnPlayerDialogueCompleted()
{
	if (InGameUI != nullptr)
	{
		InGameUI->OnDialogueCompleted.RemoveDynamic(
			this, &ARPGPlayerController::OnPlayerDialogueCompleted);
	}

	SetCinematicMode(false, true, true);
}
