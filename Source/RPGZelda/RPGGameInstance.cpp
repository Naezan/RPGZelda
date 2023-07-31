// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"
#include "RPGSaveGame.h"
#include "RPGPlayerController.h"
#include "InteractiveNPCCharacter.h"
#include "QuestChestActor.h"
#include "LinkCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

URPGGameInstance::URPGGameInstance()
{
	SaveSlotName = TEXT("SaveData");
	SaveUserIndex = 0;
}

URPGSaveGame* URPGGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void URPGGameInstance::WriteSaveGame()
{
	URPGSaveGame* NewGameData = NewObject<URPGSaveGame>();
	CurrentSaveGame = NewGameData;
	CurrentSaveGame->SavedPlayers.Empty();
	CurrentSaveGame->SavedNPCs.Empty();
	CurrentSaveGame->SavedChests.Empty();

	//플레이어 정보 가져와서 저장 플레이어 컨트롤러에서 세이브하는 함수
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ARPGPlayerController* PC = Cast<ARPGPlayerController>(*Iterator);

		if (PC)
		{
			//플레이어 컨트롤러에서 플레이어의 정보를가져와서 전달해줌
			FPlayerSaveData PlayerData;
			ALinkCharacter* NewPlayer = PC->GetPlayerCharacter();
			if (NewPlayer->IsValidLowLevel())
			{
				PlayerData.Health = NewPlayer->GetHealth();

				if (NewPlayer->EquippedRPGItems.Num() != 0)
				{
					for (int i = 0; i < NewPlayer->EquippedRPGItems.Num(); ++i)
					{
						PlayerData.GrantedItems.Add(NewPlayer->EquippedRPGItems[i]);
					}
					UE_LOG(LogTemp, Log, TEXT("Save Item Number %d"), NewPlayer->EquippedRPGItems.Num());
				}
			}

			CurrentSaveGame->SavedPlayers.Add(PlayerData);
			break;
		}
	}

	//npc정보 가져와서 저장
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AInteractiveNPCCharacter* NPC = Cast<AInteractiveNPCCharacter>(*It);
		AQuestChestActor* Chest = Cast<AQuestChestActor>(*It);
		if (NPC)
		{
			//bIsQuestDone
			FNPCSaveData NPCData;
			NPCData.NPCName = NPC->GetFName();
			NPCData.bTalkEnd = NPC->GetIsQuestDone();
			NPCData.Transform = NPC->GetActorTransform();

			UE_LOG(LogTemp, Log, TEXT("NPC Name %s"), *NPCData.NPCName.ToString());

			CurrentSaveGame->SavedNPCs.Add(NPCData);
		}
		else if (Chest)
		{
			//bIsInteract
			FChestSaveData ChestData;
			ChestData.ChestName = Chest->GetFName();
			ChestData.bIsOpen = Chest->GetIsInteract();
			ChestData.bMoveComplete = Chest->GetMoveComplete();
			ChestData.bAlpha = Chest->GetAlpha();

			UE_LOG(LogTemp, Log, TEXT("Chest Name %s"), *ChestData.ChestName.ToString());

			CurrentSaveGame->SavedChests.Add(ChestData);
		}
	}

	//상자정보 가져와서 저장

	UGameplayStatics::SaveGameToSlot(
		CurrentSaveGame, SaveSlotName, SaveUserIndex);

	UE_LOG(LogTemp, Log, TEXT("Saved Game Data."));
}

void URPGGameInstance::LoadSaveGame(APlayerController* PlayerController)
{
	if (UGameplayStatics::DoesSaveGameExist(
		SaveSlotName, SaveUserIndex))
	{
		CurrentSaveGame = Cast<URPGSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));

		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded Game Data."));

		//플레이어 정보 엑터에 할당
		ARPGPlayerController* PC = Cast<ARPGPlayerController>(PlayerController);
		if (PC)
		{
			if (CurrentSaveGame->SavedPlayers.Num() == 0)
				return;

			UE_LOG(LogTemp, Log, TEXT("Players Count %d"), CurrentSaveGame->SavedPlayers.Num());

			for (FPlayerSaveData FoundData : CurrentSaveGame->SavedPlayers)
			{
				ALinkCharacter* NewPlayer = PC->GetPlayerCharacter();
				if (NewPlayer != nullptr)
				{
					NewPlayer->SetHealth(FoundData.Health);
					UE_LOG(LogTemp, Log, TEXT("Load Health %f"), NewPlayer->GetHealth());
					if (FoundData.GrantedItems.Num() != 0)
					{
						for (int i = 0; i < FoundData.GrantedItems.Num(); ++i)
						{
							NewPlayer->EquippedRPGItems.Add(FoundData.GrantedItems[i]);
						}
						UE_LOG(LogTemp, Log, TEXT("Load Item Numbers %d"), FoundData.GrantedItems.Num());
					}

					break;
				}
				else
					UE_LOG(LogTemp, Log, TEXT("Cant Find Player Character"));
			}
		}

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AInteractiveNPCCharacter* NPC = Cast<AInteractiveNPCCharacter>(*It);
			AQuestChestActor* Chest = Cast<AQuestChestActor>(*It);

			//npc정보 엑터에 할당
			if (NPC)
			{
				for (FNPCSaveData ActorData : CurrentSaveGame->SavedNPCs)
				{
					if (ActorData.NPCName == NPC->GetFName())
					{
						NPC->SetIsQuestDone(ActorData.bTalkEnd);
						NPC->SetActorTransform(ActorData.Transform);
						break;
					}
				}
			}
			//상자 정보 엑터에 할당
			else if (Chest)
			{
				for (FChestSaveData ActorData : CurrentSaveGame->SavedChests)
				{
					if (ActorData.ChestName == Chest->GetFName())
					{
						Chest->SetIsInteract(ActorData.bIsOpen);
						Chest->SetMoveComplete(ActorData.bMoveComplete);
						Chest->SetAlpha(ActorData.bAlpha);
						break;
					}
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<URPGSaveGame>(
			UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
