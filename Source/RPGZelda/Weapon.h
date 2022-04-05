// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UCapsuleComponent;
class ALinkCharacter;

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	//소모성인지 아닌지
	//소모성이면 갯수
	//데미지가 있는지 없는지
	//

	FWeaponData()
	{
		Damage = 10.f;
	}
};

UCLASS()
class RPGZELDA_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	FWeaponData WeaponConfig;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	ALinkCharacter* MyCharacter;

public:
	void SetOwningPawn(ALinkCharacter* NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnEquip();

	FWeaponData GetWeaponConfig();
	UCapsuleComponent* GetCapsuleComp();
};
