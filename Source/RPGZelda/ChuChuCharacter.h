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

	// Take Damage
	virtual float TakeDamage(
		float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;

	virtual void OnAttackEnd() override;

	UFUNCTION()
	void AttackTakeDamage();

	virtual void OnDead() override;

public:
	//UPROPERTY(EditDefaultsOnly, Category = Animation)
	//UAnimMontage* AttackAnim;

public:

	static const float MAX_HP;
};
