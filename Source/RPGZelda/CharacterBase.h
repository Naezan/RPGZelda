// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGAbilitySystemComponent.h"
#include "RPGAttributeSet.h"
#include "CharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class UAnimMontage;
class UAttributeComponent;
class UGameplayEffect;

UCLASS()
class RPGZELDA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
		virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
		virtual float GetMaxHealth() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
		virtual float GetMoveSpeed() const;

	virtual void SetHealth(float Health);

protected:
	/** The component used to handle ability system interactions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
		URPGAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
		URPGAttributeSet* AttributeSet;

	/** If true we have initialized our abilities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
		int32 bAbilitiesInitialized;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TArray<TSubclassOf<URPGGameplayAbility>> GameplayAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnDead();

	//부모는 공격 어빌리티 호출
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
		virtual void OnAttackEnd();

	UFUNCTION(BlueprintImplementableEvent)
		void OnAttacking();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void EventBeginAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void EventEndAttack();



public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHp = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAttacking;

	//움직임 제어
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStopMoving;

	UPROPERTY()
		FTimerHandle DisableHpWidgetTimer;

	UPROPERTY()
		FTimerHandle DestoryActorTimer;

	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnDeadDelegate OnDeadDelegate;

public:
	UFUNCTION(BlueprintCallable)
		bool IsAlive();

	UFUNCTION(BlueprintCallable)
		bool GetStopMoving();

protected:
	//Ability

	/**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped based on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character that initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/** Apply the startup gameplay abilities and effects */
	virtual void AddStartupGameplayAbilities();
	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	// Friended to allow access to handle functions above
	friend URPGAttributeSet;
};
