// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/UnrealMathUtility.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Weapon.h"
#include "InteractiveNPCCharacter.h"
#include "RPGPlayerController.h"

#include "InteractComponent.h"

// Sets default values
ALinkCharacter::ALinkCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));

	//AI가 인식하는 컴포넌트
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>
		(TEXT("AIPerceptionSource"));

	//스프링암 컴포넌트 생성
	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 50.f),
		FRotator(-60.f, 0.f, 0.f));
	CameraSpringArmComp->TargetArmLength = 400.f;
	CameraSpringArmComp->bEnableCameraLag = true;
	CameraSpringArmComp->CameraLagSpeed = 5.f;
	CameraSpringArmComp->SetUsingAbsoluteRotation(true);//스프링암의 좌표가 월드의 변화에 따라감
	CameraSpringArmComp->SetupAttachment((USceneComponent*)GetCapsuleComponent());

	//카메라 컴포넌트 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	CameraComp->SetupAttachment(CameraSpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	WeaponComp->SetupAttachment(GetMesh());

	PetComp = CreateDefaultSubobject<USceneComponent>(TEXT("PetComp"));
	PetComp->SetupAttachment(GetMesh());
	PetComp->SetRelativeLocation(FVector(30.f, 80.f, 100.f));

	LookAtPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LookAtPoint"));
	LookAtPoint->SetupAttachment(GetMesh());
	LookAtPoint->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	InteractionComp = CreateDefaultSubobject<UInteractComponent>("InteractionComp");

	//플레이어 소유권 부여
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 70.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 640.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	MaxWalkSpeed = 600.f;
	MaxRunSpeed = 1250.f;

	//무기 생성
	CurrentWeapon = NULL;

	IsDead = false;
}

// Called when the game starts or when spawned
void ALinkCharacter::BeginPlay()
{
	Super::BeginPlay();

	//테스트
	//GiveDefaultWeapon();
}

void ALinkCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ALinkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	HandleZoom(DeltaTime);
}

// Called to bind functionality to input
void ALinkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALinkCharacter::RotateCharacter()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		float LocationX, LocationY;
		PlayerController->GetMousePosition(LocationX, LocationY);

		FVector2D MousePosition(LocationX, LocationY);
		FHitResult HitResult;
		if (PlayerController->GetHitResultAtScreenPosition(
			MousePosition, ECC_Visibility, true, HitResult))
		{
			FVector TargetLocation = FVector(
				HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z);
			FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(), TargetLocation);
			FRotator NewRotator = FRotator(GetActorRotation().Pitch,
				(LookAtRotator.Yaw), GetActorRotation().Roll);

			SetActorRotation(NewRotator);
		}
	}
}

void ALinkCharacter::MoveForward(float Value)
{
	//AddMovementInput(GetActorForwardVector() * Value);
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsStopMoving)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALinkCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector() * Value);
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsStopMoving)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALinkCharacter::MouseX(float Value)
{
}

void ALinkCharacter::MouseY(float Value)
{
}

void ALinkCharacter::Zooming(float Value)
{
	if (Value > 0)
	{
		ZoomOut();
	}
	else if (Value < 0)
	{
		ZoomIn();
	}

	//const float NewFieldOfView = CameraComp->FieldOfView + Value * 10.f;
	//CameraComp->FieldOfView = FMath::Clamp(NewFieldOfView, 60.0f, 90.0f);
	//
	//const float NewTargetArmLength = CameraSpringArmComp->TargetArmLength + Value * 10.f;
	//CameraSpringArmComp->TargetArmLength = FMath::Clamp(NewTargetArmLength, 300.0f, 400.0f);
	//
	////스프링암 로테이션 조절
	//const float NewRotator = CameraSpringArmComp->GetRelativeRotation().Pitch + Value * (-10.f);
	//CameraSpringArmComp->SetRelativeRotation(FRotator(FMath::Clamp(NewRotator, -60.f, -20.f), 0.f, 0.f));
}

bool ALinkCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		return InteractionComp->PrimaryInteract();
	}

	return false;
}

void ALinkCharacter::StartJump()
{
	bPressedJump = true;
}

void ALinkCharacter::StopJump()
{
	bPressedJump = false;
}

void ALinkCharacter::StartRun()
{
	if (!bPressedJump)
	{
		bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
	}
}

void ALinkCharacter::StopRun()
{
	if (!bPressedJump)
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

bool ALinkCharacter::GetIsRunning()
{
	return bIsRunning;
}

void ALinkCharacter::ProcessWeaponPickup(AWeapon* Weapon)
{
	//
}

void ALinkCharacter::NextWeapon()
{
	//
}

void ALinkCharacter::PrevWeapon()
{
	//
}

void ALinkCharacter::Attack()
{
	Super::Attack();

	if (CurrentWeapon != nullptr && !IsAttacking)
	{
		//몸체 회전
		RotateCharacter();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "No Weapon in Hand");
	}
}

//부모에서 호출해주면 얘가 호출됨
void ALinkCharacter::OnAttackEnd()
{
	if (IsAttacking == false)
		return;

		//isAttacking 0.3초뒤 false로 바꿔주는 함수
	MyPlayerController->PlayerAttackEnd();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, IsAttacking ? TEXT("true") : TEXT("false"));
}

void ALinkCharacter::AttackTakeDamage(AActor* OtherActor)
{
	//
}

void ALinkCharacter::OnDead()
{
	Super::OnDead();
}

void ALinkCharacter::SetInteractiveInRange(ANPCCharacterBase* Interactive)
{
	if (Interactive == nullptr)
		return;

	if (CurrentInteractive != Interactive)
		CurrentInteractive = Interactive;
}

void ALinkCharacter::ClearInteractiveInRange(ANPCCharacterBase* Interactive)
{
	if (Interactive == nullptr)
		return;

	if (CurrentInteractive == Interactive)
		CurrentInteractive = nullptr;
}

void ALinkCharacter::LookAtUpdate(float DeltaTime)
{
	if (CurrentInteractive != nullptr)
	{

	}
}

void ALinkCharacter::ZoomIn()
{
	if (CameraZoomState + 1 == 3)
		return;

	if (CameraZoomState + 1 < 3)
	{
		CameraZoomState = CameraZoomState + 1;
	}

	bTransitionZoomOut = false;
	bTransitionZoomIn = true;
	CurrentCameraZoom = 0.f;
}

void ALinkCharacter::ZoomOut()
{
	if (CameraZoomState - 1 == 0)
		return;

	if (CameraZoomState - 1 > -1)
	{
		CameraZoomState = CameraZoomState - 1;
	}
	bTransitionZoomOut = true;
	bTransitionZoomIn = false;
	CurrentCameraZoom = 0.f;
}

void ALinkCharacter::HandleZoom(float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZoomDelta : %f"), CurrentCameraZoom);
	if (bTransitionZoomIn)
	{
		//스프링암 타겟거리
		CurrentCameraZoom += (ZoomTransitionSpeed * DeltaSeconds);
		CameraSpringArmComp->TargetArmLength = FMath::Lerp(CameraSpringArmComp->TargetArmLength, TargetArmLength, CurrentCameraZoom);
		CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, TargetFOV, CurrentCameraZoom);
		float NewRotator = FMath::Lerp(CameraSpringArmComp->GetRelativeRotation().Pitch, TargetPitch, CurrentCameraZoom);
		CameraSpringArmComp->SetRelativeRotation(FRotator(NewRotator, 0.f, 0.f));

		if (CurrentCameraZoom >= 1.f)
		{
			bTransitionZoomIn = false;
		}
	}
	if (bTransitionZoomOut)
	{
		//스프링암 타겟거리
		CurrentCameraZoom += (ZoomTransitionSpeed * DeltaSeconds);
		CameraSpringArmComp->TargetArmLength = FMath::Lerp(CameraSpringArmComp->TargetArmLength, InitialArmLength, CurrentCameraZoom);
		CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, InitialFOV, CurrentCameraZoom);
		float NewRotator = FMath::Lerp(CameraSpringArmComp->GetRelativeRotation().Pitch, InitialPitch, CurrentCameraZoom);
		CameraSpringArmComp->SetRelativeRotation(FRotator(NewRotator, 0.f, 0.f));

		if (CurrentCameraZoom >= 1.f)
		{
			bTransitionZoomOut = false;
		}
	}
}
