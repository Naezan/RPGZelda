// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "LinkCharacter.h"

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

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetOwningPawn(ALinkCharacter* NewOwner)
{
	if (MyCharacter != NewOwner)
	{
		MyCharacter = NewOwner;
	}
}

void AWeapon::AttachToPlayer()
{
	if (MyCharacter)
	{
		//기존에 달려있는 무기 제거
		DetachFromPlayer();

		USkeletalMeshComponent* Character = MyCharacter->GetMesh();
		Mesh->AttachToComponent(Character, 
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Weapon");
	}
}

void AWeapon::DetachFromPlayer()
{
	Mesh->DetachFromParent();
}

void AWeapon::OnEquip()
{
	//충돌체 설정
	AttachToPlayer();
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

FWeaponData AWeapon::GetWeaponConfig()
{
	return WeaponConfig;
}

UCapsuleComponent* AWeapon::GetCapsuleComp()
{
	return CapsuleComp;
}

