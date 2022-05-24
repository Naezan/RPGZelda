// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PhantomCharacter.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Peace UMETA(DisplayName = "Peace"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Search UMETA(DisplayName = "Search"),
	Dead UMETA(DisplayName = "Dead"),
};

UCLASS()
class RPGZELDA_API APhantomCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APhantomCharacter();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMonsterState PhantomState;

public:
	UFUNCTION(BlueprintCallable)
		EMonsterState SetBossState(EMonsterState MonsterState);
};
