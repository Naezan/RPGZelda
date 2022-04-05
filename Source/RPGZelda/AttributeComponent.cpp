// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "RPGZeldaGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("su.DamageMultiplier"), 1.0f, 
	TEXT("Global Damage Modifier for Attribute Component."), 
	ECVF_Cheat);

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
}

bool UAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}


bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}


float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// Died
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			AGameModeBase* GM = GetWorld()->GetAuthGameMode<AGameModeBase>();
			if (GM)
			{
				//GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

float UAttributeComponent::GetRage() const
{
	return Rage;
}

bool UAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UAttributeComponent>(FromActor->GetComponentByClass(UAttributeComponent::StaticClass()));
	}

	return nullptr;
}


bool UAttributeComponent::IsActorAlive(AActor* Actor)
{
	UAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

void UAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}


void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeComponent, Health);
	DOREPLIFETIME(UAttributeComponent, HealthMax);

	DOREPLIFETIME(UAttributeComponent, Rage);
	DOREPLIFETIME(UAttributeComponent, RageMax);
}