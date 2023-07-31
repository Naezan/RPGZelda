// Fill out your copyright notice in the Description page of Project Settings.


#include "BlaazCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABlaazCharacter::ABlaazCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//AIControllerClass = AChuChuAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(34.f, 80.f);

	//Lerp 움직임 셋팅
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	IsDead = false;

	MaxSpawnCount = 3;
}

void ABlaazCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlaazCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlaazCharacter::Attack()
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

void ABlaazCharacter::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

void ABlaazCharacter::OnDead()
{
	Super::OnDead();
}

EBossState ABlaazCharacter::SetBossState(EBossState BossState)
{
	BlaazState = BossState;

	return BlaazState;
}
