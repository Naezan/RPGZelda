// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuChuCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChuChuAIController.h"
#include "Animation/AnimInstance.h"


AChuChuCharacter::AChuChuCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AChuChuAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(34.f, 60.f);

	//Lerp 움직임 셋팅
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	IsDead = false;
}

void AChuChuCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AChuChuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChuChuCharacter::Attack()
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

void AChuChuCharacter::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

void AChuChuCharacter::OnDead()
{
	Super::OnDead();
}