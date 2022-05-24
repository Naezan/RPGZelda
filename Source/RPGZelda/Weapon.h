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

protected:
	UPROPERTY(EditDefaultsOnly)
	FWeaponData WeaponConfig;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ALinkCharacter* MyCharacter;

	//PlayerAttack
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAttacking = false;

public:
	void SetOwningPawn(ALinkCharacter* NewOwner);

	FWeaponData GetWeaponConfig();
	UCapsuleComponent* GetCapsuleComp();
	
	//PlayerAttack
	//블루프린트에서 추가적으로 구현해야할것들 구현
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Attack")
	void BeginAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Attack")
	void KeepAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Attack")
	void EndAttack();
};
