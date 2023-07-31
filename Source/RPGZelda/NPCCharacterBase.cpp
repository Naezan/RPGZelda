// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacterBase.h"
#include "Components/BoxComponent.h"
#include "LinkCharacter.h"
#include "RPGPlayerController.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ANPCCharacterBase::ANPCCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LookAtPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LookAtPoint"));
	LookAtPoint->SetupAttachment(RootComponent);

	InteractiveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveCollision"));
	InteractiveCollision->SetupAttachment(RootComponent);

	InteractiveCollision->OnComponentBeginOverlap.AddDynamic(
		this, &ANPCCharacterBase::OnOverlapBegin);
	InteractiveCollision->OnComponentEndOverlap.AddDynamic(
		this, &ANPCCharacterBase::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	bIsEnabled = true;
}

// Called every frame
void ANPCCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtPlayer(DeltaTime);
}

// Called to bind functionality to input
void ANPCCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCCharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsEnabled) return;

	CurrentCharacter = Cast<ALinkCharacter>(OtherActor);

	if (CurrentCharacter)
	{
		CurrentPlayerController = Cast<ARPGPlayerController>(CurrentCharacter->GetController());

		SetInteractive();
	}
}

void ANPCCharacterBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsEnabled) return;

	CurrentCharacter = Cast<ALinkCharacter>(OtherActor);

	if (CurrentCharacter)
	{
		ClearInterative();
	}
}

void ANPCCharacterBase::LookAtPlayer(float DeltaTime)
{
	if (CurrentCharacter != nullptr)
	{
		FVector HeadSocketLocation = GetMesh()->GetSocketLocation("LookAtHead");

		FVector LookAtLocation = CurrentCharacter->GetLookAtPoint()->GetComponentLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HeadSocketLocation, LookAtLocation);

		FRotator RotatorDiff = FRotator(
			LookAtRotation.Pitch - GetActorRotation().Pitch,
			LookAtRotation.Yaw - GetActorRotation().Yaw,
			0.f
		);

		FRotator TargetRotation = FRotator(0.f, RotatorDiff.Yaw > 180 ? RotatorDiff.Yaw - 360.f : RotatorDiff.Yaw, -RotatorDiff.Pitch);

		if (FMath::Abs(TargetRotation.Pitch) > MaxPitchRotation)
		{
			TargetRotation.Pitch = 0.f;
		}

		if (FMath::Abs(TargetRotation.Yaw) > MaxYawRotation)
		{
			TargetRotation.Yaw = 0.f;
		}

		CurrentHeadRotation = FMath::RInterpTo(CurrentHeadRotation, TargetRotation, DeltaTime, 4.f);

		SetHeadRotation(CurrentHeadRotation);
	}
	else if (bCanLookAt)
	{
		CurrentHeadRotation = FMath::RInterpTo(CurrentHeadRotation, FRotator::ZeroRotator, DeltaTime, 4.f);

		SetHeadRotation(CurrentHeadRotation);

		if (CurrentHeadRotation.IsNearlyZero())
		{
			CurrentHeadRotation = FRotator::ZeroRotator;
			SetHeadRotation(CurrentHeadRotation);

			SetHeadRotationAlpha(0.f);
			bCanLookAt = false;
		}
	}
}

void ANPCCharacterBase::SetEnable(bool Enable)
{
	bIsEnabled = Enable;

	if (bIsEnabled)
	{
		InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ANPCCharacterBase::Interact()
{
	if (!bIsEnabled) return;
}

void ANPCCharacterBase::SetInteractive()
{
	CurrentCharacter->SetInteractiveInRange(this);

	bCanLookAt = true;

	SetHeadRotation(CurrentHeadRotation);

	SetHeadRotationAlpha(1.f);
}

void ANPCCharacterBase::ClearInterative()
{
	CurrentCharacter->ClearInteractiveInRange(this);

	CurrentPlayerController = nullptr;

	CurrentCharacter = nullptr;
}

