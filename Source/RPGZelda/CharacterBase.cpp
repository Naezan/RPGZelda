// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemGlobals.h"
#include "RPGAbilitySystemComponent.h"
#include "RPGGameplayAbility.h"
#include "RPGAttributeSet.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	//AI소유시점
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//사운트 셋팅
	TakeDownAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDownAudioComponent"));
	TakeDownAudioComponent->bAutoActivate = false;
	TakeDownAudioComponent->SetupAttachment(RootComponent);

	DieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioComponent"));
	DieAudioComponent->bAutoActivate = false;
	DieAudioComponent->SetupAttachment(RootComponent);
	DieAudioComponent->bOverrideAttenuation = true;

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

	bAbilitiesInitialized = false;

	//죽을경우에 호출되는 함수를 델리게이트에 미리 넣어놓음
	OnDeadDelegate.AddUObject(this, &ACharacterBase::OnDead);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAttributes();

	AddStartupGameplayAbilities();
}

void ACharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ACharacterBase::GetHealth() const
{
	if (!AttributeSet)
		return 1.f;

	return AttributeSet->GetHealth();
}

float ACharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ACharacterBase::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Sound
	if (TakeDownSoundCue->IsValidLowLevelFast())
		TakeDownAudioComponent->SetSound(TakeDownSoundCue);

	if (DieSoundCue->IsValidLowLevelFast())
		DieAudioComponent->SetSound(DieSoundCue);

	SetHealth(GetMaxHealth());

	/*for (TSubclassOf<URPGGameplayAbility>& Ability : GameplayAbilities)
	{
		FName abilityname = Ability->GetFName();
		UE_LOG(LogTemp, Warning, TEXT("Ability : %s"), *abilityname.ToString());
	}*/

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::OnDead()
{
	// Only runs on Server 어빌리티 삭제
	RemoveCharacterAbilities();

	//데스 오디오 호출
	if (DieAudioComponent)
	{
		DieAudioComponent->Play();
	}

	if (AbilitySystemComponent->IsValidLowLevel())
	{
		AbilitySystemComponent->CancelAllAbilities();

		//데드 이펙트 테그
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	DisableInput(PC);

	SetLifeSpan(5.0f);
}

void ACharacterBase::Attack()
{
	//게임 어빌리티 사용하는 용도
	OnAttacking();
}

//노티파이에서 호출
void ACharacterBase::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

bool ACharacterBase::IsAlive()
{
	return !IsDead;
}

bool ACharacterBase::GetStopMoving()
{
	return bStopMoving;
}

void ACharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorPawn, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);
}

void ACharacterBase::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// We only call the BP callback if this is not the initial ability setup
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ACharacterBase::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	UE_LOG(LogTemp, Warning, TEXT("PossessedBy"));

	if (HasAuthority() && !bAbilitiesInitialized)
	{
		for (TSubclassOf<URPGGameplayAbility>& Ability : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	bAbilitiesInitialized = true;
}

void ACharacterBase::RemoveCharacterAbilities()
{
	if (HasAuthority() ||
		!AbilitySystemComponent->IsValidLowLevel())
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && GameplayAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
}

void ACharacterBase::InitializeAttributes()
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void ACharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority ||
		AbilitySystemComponent == nullptr)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void ACharacterBase::SetHealth(float Health)
{
	if (AttributeSet != nullptr)
	{
		AttributeSet->SetHealth(Health);
	}
}
