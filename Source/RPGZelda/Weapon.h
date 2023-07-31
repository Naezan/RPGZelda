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

	//�Ҹ����� �ƴ���
	//�Ҹ��̸� ����
	//�������� �ִ��� ������
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
	//�������Ʈ���� �߰������� �����ؾ��Ұ͵� ����
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Attack")
	void BeginAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Attack")
	void KeepAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Attack")
	void EndAttack();
};
