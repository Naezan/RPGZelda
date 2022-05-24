// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "IngameUI.h"
#include "Blueprint/UserWidget.h"
#include "LinkCharacter.h"
#include "Weapon.h"
#include "UObject/ConstructorHelpers.h"
#include "InteractiveNPCCharacter.h"

#include "RPGGameInstance.h"
#include "RPGItem.h"
#include "RPGSaveGame.h"

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
	//LoadRPGItems();

	Super::BeginPlay();

	APawn* const pawn = GetPawn();

	if (pawn != nullptr)
	{
		MainCharacter = Cast<ALinkCharacter>(pawn);
		MainCharacter->MyPlayerController = this;
		UE_LOG(LogTemp, Log, TEXT("Character"));
	}

	if (InGameUIClass != nullptr)
	{
		InGameUI = CreateWidget<UIngameUI>(this, InGameUIClass);
	}

	if (InGameUI != nullptr)
	{
		InGameUI->AddToViewport();

		//ó���� �Ⱥ��̵���
		InGameUI->OnAnimationHideMessageUI();

		//InGameUI->AnimateMessage("Link", "This is Animated Message");

		//test
		//InGameUI->SetMessage("Hello World!");
		//InGameUI->SetCharacterName("Link");
		//InGameUI->OnAnimationShowMessageUI();

		/*if (PlayerDialogue != nullptr)
		{
			SetCinematicMode(true, true, true);
			InGameUI->InitializeDialogue(PlayerDialogue);

			InGameUI->OnDialogueCompleted.AddDynamic(
				this, &ARPGPlayerController::OnPlayerDialogueCompleted);
		}*/
	}

	InitSaveData();
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bInputMove)
	{
		MoveToMouseCursor();
	}
	//UE_LOG(LogTemp, Warning, TEXT("Y : %s"), bInputMove ? TEXT("true") : TEXT("false"));
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InputMove", IE_Pressed, this, &ARPGPlayerController::StartInputMove);
	InputComponent->BindAction("InputMove", IE_Released, this, &ARPGPlayerController::StopInputMove);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ARPGPlayerController::Interact);

	InputComponent->BindAction("KeyUp", IE_Pressed, this, &ARPGPlayerController::OnKeyUp).bConsumeInput = false;
	InputComponent->BindAction("KeyDown", IE_Pressed, this, &ARPGPlayerController::OnKeyDown).bConsumeInput = false;

	InputComponent->BindAction("LeftChangeItem", IE_Pressed, this, &ARPGPlayerController::OnLeftChangeItem);
	InputComponent->BindAction("RightChangeItem", IE_Pressed, this, &ARPGPlayerController::OnRightChangeItem);

	//�÷��̾� �Է�
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARPGPlayerController::PlayerStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ARPGPlayerController::PlayerStopJump);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ARPGPlayerController::BeginPlayerAttack);
	//InputComponent->BindAction("Attack", IE_Repeat, this, &ARPGPlayerController::KeepPlayerAttack);
	InputComponent->BindAction("Attack", IE_Released, this, &ARPGPlayerController::EndPlayerAttack);

	InputComponent->BindAction("Run", IE_Pressed, this, &ARPGPlayerController::PlayerStartRun);
	InputComponent->BindAction("Run", IE_Released, this, &ARPGPlayerController::PlayerStopRun);

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ARPGPlayerController::PlayerMoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &ARPGPlayerController::PlayerMoveRight);

	InputComponent->BindAxis("Zoom", this, &ARPGPlayerController::PlayerZooming);

	UE_LOG(LogTemp, Warning, TEXT("InputComponent"));
}

void ARPGPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	bool bIsDragButtonDown_Old = bIsAttackButtonDown;
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);

	if (bIsAttackButtonDown && (bIsDragButtonDown_Old == bIsAttackButtonDown))
	{
		KeepPlayerAttack();
	}
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
	if (MainCharacter != nullptr)
	{
		//������Ʈ ��ȣ�ۿ�
		bool IsFocusActor = MainCharacter->PrimaryInteract();

		//if (IsFocusActor)
		//	return;

		//��ȭ
		if (MainCharacter->GetInteractive() != nullptr)
			MainCharacter->GetInteractive()->Interact();
	}

	if (InGameUI != nullptr)
	{
		InGameUI->InteractUI();
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

void ARPGPlayerController::OnLeftChangeItem()
{
}

void ARPGPlayerController::OnRightChangeItem()
{
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

void ARPGPlayerController::PlayerMoveForward(float Value)
{
	if (MainCharacter)
	{
		MainCharacter->MoveForward(Value);
	}
}

void ARPGPlayerController::PlayerMoveRight(float Value)
{
	if (MainCharacter)
	{
		MainCharacter->MoveRight(Value);
	}
}

void ARPGPlayerController::PlayerZooming(float Value)
{
	if (MainCharacter)
	{
		MainCharacter->Zooming(Value);
	}
}

void ARPGPlayerController::PlayerStartJump()
{
	if (MainCharacter)
	{
		MainCharacter->StartJump();
	}
}

void ARPGPlayerController::PlayerStopJump()
{
	if (MainCharacter)
	{
		MainCharacter->StopJump();
	}
}

void ARPGPlayerController::PlayerStartRun()
{
	if (MainCharacter)
	{
		MainCharacter->StartRun();
	}
}

void ARPGPlayerController::PlayerStopRun()
{
	if (MainCharacter)
	{
		MainCharacter->StopRun();
	}
}

void ARPGPlayerController::BeginPlayerAttack()
{
//�����߿��� ����Ű �ȵǵ���
	if (MainCharacter != nullptr && MainCharacter->CurrentWeapon != nullptr && !MainCharacter->IsAttacking)
	{
		bInputMove = false;
		bIsAttackButtonDown = true;

		//�ٶ󺸴¹��� �Ĵٺ���
		MainCharacter->Attack();

		//���Ⱑ���°�쿡 ������������
		MainCharacter->CurrentWeapon->SetOwningPawn(MainCharacter);
		//�����Լ��� �������Ʈ������ ���������
		MainCharacter->CurrentWeapon->BeginAttack();
	}
}

void ARPGPlayerController::KeepPlayerAttack()
{
	if (MainCharacter != nullptr && MainCharacter->CurrentWeapon != nullptr)
	{
		MainCharacter->CurrentWeapon->KeepAttack();
	}
}

void ARPGPlayerController::EndPlayerAttack()
{
	if (MainCharacter != nullptr && MainCharacter->CurrentWeapon != nullptr)
	{
		bIsAttackButtonDown = false;

		MainCharacter->CurrentWeapon->EndAttack();
	}
}

void ARPGPlayerController::PlayerAttackEnd()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Enable();
			MainCharacter->IsAttacking = false;
			MainCharacter->OnAttackEndDelegate.Broadcast();
		}), 0.1f, false);
}

ALinkCharacter* ARPGPlayerController::GetPlayerCharacter()
{
	return MainCharacter;
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

void ARPGPlayerController::Disable()
{
	DisableInput(Cast<APlayerController>(this));
}

void ARPGPlayerController::Enable()
{
	EnableInput(Cast<APlayerController>(this));
}

bool ARPGPlayerController::LoadRPGItems()
{
	GrantedItems.Reset();

	// Fill in slots from game instance
	UWorld* World = GetWorld();
	URPGGameInstance* GameInstance = World ? World->GetGameInstance<URPGGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemsPerType)
	{
		for (int32 IdxNumber = 0; IdxNumber < Pair.Value; IdxNumber++)
		{
			GrantedItems.Add(FRPGItemInfo(Pair.Key, IdxNumber), nullptr);
		}
	}

	return true;
}

void ARPGPlayerController::GetGrantedItems(TArray<URPGItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	for (TPair<FRPGItemInfo, URPGItem*>& Pair : GrantedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

bool ARPGPlayerController::SetGrantedItem(URPGItem* Item)
{
	for (TPair<FRPGItemInfo, URPGItem*>& Pair : GrantedItems)
	{
		if (Pair.Value != nullptr && Pair.Key.ItemType == Item->ItemType)
		{
			// Add to new item
			Pair.Value = Item;
		}
	}

	return true;
}
