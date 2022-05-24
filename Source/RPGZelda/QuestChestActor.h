// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActorBase.h"
#include "QuestChestActor.generated.h"

class AInteractiveNPCCharacter;
class UCameraComponent;
class USpringArmComponent;
class UDataTable;

class ALinkCharacter;
class ARPGPlayerController;

class AStaticMeshActor;

/**
 *
 */
UCLASS()
class RPGZELDA_API AQuestChestActor : public AInteractiveActorBase
{
	GENERATED_BODY()

public:
	AQuestChestActor();

	//virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* PickUpPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* ItemSpawnPoint;

	//퀘스트 npc정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSoftObjectPtr<AInteractiveNPCCharacter> TargetNPC;

	//상자가 가지고 있는 아이템 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class URPGItem* AcquireItem;

	//아이템을 먹었을때 띄울 자막 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UDataTable* Dialogue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialogue")
		ALinkCharacter* PlayerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialogue")
		ARPGPlayerController* CurrentPlayerController;

	//상자열때 생성할 임시매쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* ItemMesh;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		AStaticMeshActor* ItemActor;

	//아이템을 먹었을때 확대할 카메라
	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
		USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
		UCameraComponent* CameraComp;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f))
		float CameraBlendTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chest")
		bool bIsInteract = false;

	//퀘스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
		bool bIsQuestComplete;

	//캐릭터 무브변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
		float Speed = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
		bool MoveComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
		FVector LastLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chest")
		float Alpha;


public:
	UFUNCTION(BlueprintCallable)
		bool GetIsInteract();
	UFUNCTION(BlueprintCallable)
		void SetIsInteract(bool bInteract);

	UFUNCTION(BlueprintCallable)
		bool GetMoveComplete() { return MoveComplete; }
	UFUNCTION(BlueprintCallable)
		void SetMoveComplete(bool bComplete);

	UFUNCTION(BlueprintCallable)
		float GetAlpha() { return Alpha; }
	UFUNCTION(BlueprintCallable)
		void SetAlpha(float bAlpha);

protected:
	UFUNCTION()
		void OnDialogueCompleted();

public:
	UFUNCTION(BlueprintCallable)
		void InteractDialogue();

	UFUNCTION(BlueprintCallable)
		void ZoomInCamera();

	UFUNCTION(BlueprintCallable)
		void MoveToPickUpPoint(AActor* MoveActor, float DeltaTime);

	//대화도중 생성된 임시 아이템 삭제함수
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Dialogue")
		void DeleteChestItem();

	UFUNCTION(BlueprintCallable)
		void SpawnStaticMeshActor(const FVector& InLocation);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Dialogue")
		void DialogueAnimEnd();
};
