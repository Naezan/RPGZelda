// Fill out your copyright notice in the Description page of Project Settings.


#include "BlaazSplitCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABlaazSplitCharacter::ABlaazSplitCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//AIControllerClass = AChuChuAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(34.f, 60.f);

	//Lerp 움직임 셋팅
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	IsDead = false;
	bStopMoving = false;
	DistanceToPoint = 0.f;
	StopDistance = 200.f;
}

void ABlaazSplitCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlaazSplitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlaazSplitCharacter::Attack()
{
	Super::Attack();

	if (!IsAttacking)
	{
		if (nullptr == AttackAnim)
			return;

		IsAttacking = true;
		PlayAnimMontage(AttackAnim);
	}
}

void ABlaazSplitCharacter::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

bool ABlaazSplitCharacter::MoveToStartPoint(FVector Forward, float DeltaTime)
{
	if (DistanceToPoint >= StopDistance)
		return true;

	FVector NewLocation = GetActorLocation();
	NewLocation += Forward * Speed * DeltaTime;
	DistanceToPoint += FVector::Dist(NewLocation, GetActorLocation());

	SetActorLocationAndRotation(NewLocation, GetActorRotation());

	return false;
}