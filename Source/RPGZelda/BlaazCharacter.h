// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "BlaazCharacter.generated.h"


class ABlaazSplitCharacter;

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Peace UMETA(DisplayName = "Peace"),
	Teleport UMETA(DisplayName = "Teleport"),
	Meteo UMETA(DisplayName = "Meteo Attack"),
	Push UMETA(DisplayName = "Push"),
	Vanish UMETA(DisplayName = "Vanish"),
	Dead UMETA(DisplayName = "Dead"),
};

UCLASS()
class RPGZELDA_API ABlaazCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlaazCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	virtual void OnAttackEnd() override;

	virtual void OnDead() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SplitMonster)
		TSubclassOf<ABlaazSplitCharacter> BlaazSplitSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SplitMonster)
		int32 SpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SplitMonster)
		int32 MaxSpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBossState BlaazState;

public:
	UFUNCTION(BlueprintCallable)
		EBossState SetBossState(EBossState bossState);
};
