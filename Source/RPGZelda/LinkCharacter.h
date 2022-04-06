// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "LinkCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class UAnimMontage;
class ANPCCharacterBase;

/**
 * 
 */
UCLASS()
class RPGZELDA_API ALinkCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALinkCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
		USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WeaponComp;

	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void MouseX(float Value);
	UFUNCTION()
		void MouseY(float Value);
	UFUNCTION()
		void Zooming(float Value);

public:
	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void StopJump();
	UFUNCTION()
		void StartRun();
	UFUNCTION()
		void StopRun();

	UFUNCTION()
		bool IsRunning();

	UPROPERTY(EditAnywhere)
		float MaxWalkSpeed;
	UPROPERTY(EditAnywhere)
		float MaxRunSpeed;
private:
	bool bIsRunning;
	bool bIsColEnd;

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere)
		AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PetComp;

	//UPROPERTY(EditDefaultsOnly, Category = Animation)
	//	UAnimMontage* CutAnim;

	void ProcessWeaponPickup(AWeapon* Weapon);

	void NextWeapon();
	void PrevWeapon();
	void EquipWeapon(AWeapon* Weapon);
	void GiveDefaultWeapon();

	virtual float TakeDamage(
		float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual	void Attack() override;

	virtual void OnAttackEnd() override;

	UFUNCTION()
		void AttackTakeDamage(AActor* OtherActor);

	virtual void OnDead() override;

	UFUNCTION() void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, 
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex);

protected:
	ANPCCharacterBase* CurrentInteractive;

public:
	FORCEINLINE ANPCCharacterBase* GetInteractive() { return CurrentInteractive; }

public:
	void SetInteractiveInRange(ANPCCharacterBase* Interactive);
	void ClearInteractiveInRange(ANPCCharacterBase* Interactive);

protected:

	void ZoomIn();

	void ZoomOut();

	void HandleZoom(float DeltaSeconds);

protected:

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float InitialArmLength = 400.f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float TargetArmLength = 300.f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float InitialFOV = 90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float TargetFOV = 60.f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float InitialPitch = -60.f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float TargetPitch = -20.f;

	UPROPERTY(EditAnywhere, Category = "Camera Zoom")
	float ZoomTransitionSpeed = 1.f;

	float CurrentCameraZoom;

	bool bTransitionZoomIn;

	bool bTransitionZoomOut;

private:

	int32 CameraZoomState = 0;

	//virtual float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/** stop playing montage */
	//virtual void StopAnimMontage(UAnimMontage* AnimMontage) override;

	/** stop playing all montages */
	//void StopAllAnimMontages();
};
