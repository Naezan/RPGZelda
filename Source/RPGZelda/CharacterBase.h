// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class UAnimMontage;
class UAttributeComponent;

UCLASS()
class RPGZELDA_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Take Damage
	virtual float TakeDamage(
		float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDead();

	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void OnAttackEnd();

	UFUNCTION(BlueprintCallable)
	void RagDollDead();

	// AiPerception Source
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionStimuliSourceComponent* AIStimuliSource;

	//Animation
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitAnim;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackAnim;

	// Sound
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundCue* TakeDownSoundCue;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundCue* DieSoundCue;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	class UAudioComponent* TakeDownAudioComponent;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	class UAudioComponent* DieAudioComponent;

	UPROPERTY(EditAnywhere, Blueprintreadwrite)
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	bool IsAttacking;

	UPROPERTY()
	FTimerHandle DisableHpWidgetTimer;

	UPROPERTY()
	FTimerHandle DestoryActorTimer;

	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnDeadDelegate OnDeadDelegate;

protected:
	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	FString CharacterName;

public:
	void ResetState();

	void SetDefaultHP(float PMaxHp);

	void SetCharacterName(FString Name);

	UFUNCTION(BlueprintCallable)
	bool IsAlive();
};
