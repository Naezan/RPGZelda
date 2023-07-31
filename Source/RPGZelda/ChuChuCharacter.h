// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ChuChuCharacter.generated.h"


/**
 * 
 */
UCLASS()
class RPGZELDA_API AChuChuCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AChuChuCharacter();

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

};
