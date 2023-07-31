// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "LinkCharacter.h"

#include "Engine/InputDelegateBinding.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComp->SetupAttachment(Mesh);
	CapsuleComp->SetCapsuleSize(20.f, 50.f);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AWeapon::SetOwningPawn(ALinkCharacter* NewOwner)
{
	if (MyCharacter != NewOwner)
	{
		MyCharacter = NewOwner;
	}
}

FWeaponData AWeapon::GetWeaponConfig()
{
	return WeaponConfig;
}

UCapsuleComponent* AWeapon::GetCapsuleComp()
{
	return CapsuleComp;
}