// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

class ALinkCharacter;
class ARPGPlayerController;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LookAt")
		USceneComponent* LookAtPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
		class UBoxComponent* InteractiveCollision;

protected:
	UPROPERTY(EditAnywhere, Category = "LookAt")
		float MaxYawRotation = 100.f;
	UPROPERTY(EditAnywhere, Category = "LookAt")
		float MaxPitchRotation = 70.f;

	FRotator CurrentHeadRotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, Category = "LookAt")
		bool bCanLookAt = false;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
		bool bZoomWhenInRange = false;

public:
	USceneComponent* GetLookAtPoint() { return LookAtPoint; }
	FORCEINLINE bool CanLookAt() { return bCanLookAt; }

	UFUNCTION(BlueprintCallable, Category = "LookAt")
	FRotator GetHeadRotation() { return CurrentHeadRotation; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LookAt")
	void SetHeadRotation(FRotator rotator);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LookAt")
	void SetHeadRotationAlpha(float alpha);

	FORCEINLINE bool ZoomWhenInRange() { return bZoomWhenInRange; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
		ALinkCharacter* CurrentCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
		ARPGPlayerController* CurrentPlayerController;

protected:
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	void LookAtPlayer(float DeltaTime);

protected:
	bool bIsEnabled;

public:
	virtual void SetEnable(bool Enable);

	virtual void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interact")
		void SetInteractive();

	UFUNCTION(BlueprintCallable, Category = "Interact")
		void ClearInterative();

	UFUNCTION(BlueprintCallable, Category = "Interact")
		ARPGPlayerController* GetCurrentPlayerController() { return CurrentPlayerController; }
};
