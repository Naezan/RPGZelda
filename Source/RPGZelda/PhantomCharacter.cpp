// Fill out your copyright notice in the Description page of Project Settings.


#include "PhantomCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

#include "PhantomAIController.h"

APhantomCharacter::APhantomCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = APhantomAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(34.f, 60.f);

	//Lerp 움직임 셋팅
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	IsDead = false;
}

void APhantomCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APhantomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhantomCharacter::Attack()
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

void APhantomCharacter::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

void APhantomCharacter::OnDead()
{
	Super::OnDead();
}

EMonsterState APhantomCharacter::SetBossState(EMonsterState MonsterState)
{
	PhantomState = MonsterState;

	return PhantomState;
}
