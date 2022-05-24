// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "BlaazSplitCharacter.generated.h"

/**
 *
 */
UCLASS()
class RPGZELDA_API ABlaazSplitCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlaazSplitCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	virtual void OnAttackEnd() override;

public:
	UFUNCTION(BlueprintCallable)
		bool MoveToStartPoint(FVector Forward, float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DistanceToPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StopDistance;
};
