// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

UCLASS()
class RPGZELDA_API ANPCCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class USceneComponent* LookAtPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class UBoxComponent* InteractiveCollision;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
		bool bCanLookAt = false;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
		bool bZoomWhenInRange = false;

public:
	FORCEINLINE USceneComponent* GetLookAtPoint() { return LookAtPoint; }
	FORCEINLINE bool CanLookAt() { return bCanLookAt; }

	FORCEINLINE bool ZoomWhenInRange() { return bZoomWhenInRange; }

protected:
	class ALinkCharacter* CurrentCharacter;

	class ARPGPlayerController* CurrentPlayerController;

protected:
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

protected:
	bool bIsEnabled;

public:
	virtual void SetEnable(bool Enable);

	virtual void Interact();
};
