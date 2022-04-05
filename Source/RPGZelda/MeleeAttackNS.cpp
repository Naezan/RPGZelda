// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackNS.h"
#include "CharacterBase.h"

void UMeleeAttackNS::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	ACharacterBase* Pawn = Cast<ACharacterBase>(MeshComp->GetOwner());

	Pawn->OnAttackEnd();
}
