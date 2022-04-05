// Fill out your copyright notice in the Description page of Project Settings.


#include "ChuChuCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChuChuAIController.h"
#include "Animation/AnimInstance.h"

const float AChuChuCharacter::MAX_HP = 50.f;

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

	CharacterName = "ChuChu";
	IsDead = false;
}

void AChuChuCharacter::BeginPlay()
{
	Super::BeginPlay();

	//체력셋팅
	SetDefaultHP(MAX_HP);
}

void AChuChuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AChuChuCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead)
		return 0.f;

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return FinalDamage;
}

void AChuChuCharacter::Attack()
{
	if (!IsAttacking)
	{
		if (nullptr == AttackAnim)
			return;

		IsAttacking = true;
		PlayAnimMontage(AttackAnim);

		//공격 딜레이
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			AttackTakeDamage();
		}), 0.5f, false);
	}
}

void AChuChuCharacter::OnAttackEnd()
{
	IsAttacking = false;
	OnAttackEndDelegate.Broadcast();
}

void AChuChuCharacter::AttackTakeDamage()
{
	if (IsAttacking)
	{
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation() + GetActorForwardVector() * 100.f,
			GetActorLocation() + GetActorForwardVector() * 200.f,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeSphere(50.f),
			Params
		);

		//충돌이 되었다면 25만큼 데미지
		if (bResult)
		{
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(20.f, DamageEvent, GetController(), this);
			}

			//몬스터 체력
			UE_LOG(LogTemp, Warning, TEXT("Monster HP : %f"), CurrentHp);

			if (HitResult.Actor.IsValid())
				UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *HitResult.Actor->GetName());
		}
	}
}

void AChuChuCharacter::OnDead()
{
	Super::OnDead();
}