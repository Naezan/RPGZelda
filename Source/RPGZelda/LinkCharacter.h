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
class UInteractComponent;
class URPGItem;

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

	void LookAtUpdate(float DeltaTime);

protected:
	//ī�޶�
	UPROPERTY(EditAnywhere)
		USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComp;

	//����
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WeaponComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LookAt")
		USceneComponent* LookAtPoint;

	UPROPERTY(VisibleAnywhere)
		UInteractComponent* InteractionComp;

	// AiPerception Source
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UAIPerceptionStimuliSourceComponent* AIStimuliSource;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	class ARPGPlayerController* MyPlayerController;

public:

	UFUNCTION(BlueprintCallable)
		void RotateCharacter();

	//���� ��
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

	//������Ʈ ��ȣ�ۿ�
	bool PrimaryInteract();

	//���� �޸���
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
		bool GetIsRunning();

	UPROPERTY(EditAnywhere)
		float MaxWalkSpeed;
	UPROPERTY(EditAnywhere)
		float MaxRunSpeed;

private:
	bool bIsRunning;

public:

	//����
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<URPGItem*> EquippedRPGItems;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* PetComp;

	//����
	void ProcessWeaponPickup(AWeapon* Weapon);

	void NextWeapon();
	void PrevWeapon();

	UFUNCTION()
		virtual	void Attack() override;

	virtual void OnAttackEnd() override;

	UFUNCTION()
		void AttackTakeDamage(AActor* OtherActor);

	virtual void OnDead() override;

	//��ȭ
protected:
	ANPCCharacterBase* CurrentInteractive;

public:
	//��ȭ
	FORCEINLINE ANPCCharacterBase* GetInteractive() { return CurrentInteractive; }

	UFUNCTION(BlueprintCallable, Category = "Interact")
		void SetInteractiveInRange(ANPCCharacterBase* Interactive);
	UFUNCTION(BlueprintCallable, Category = "Interact")
		void ClearInteractiveInRange(ANPCCharacterBase* Interactive);

	USceneComponent* GetLookAtPoint() { return LookAtPoint; }

	//��
protected:

	void ZoomIn();

	void ZoomOut();

	void HandleZoom(float DeltaSeconds);

	//����
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

	//��
private:

	int32 CameraZoomState = 0;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest")
	bool bIsQuestComplete = false;
};
