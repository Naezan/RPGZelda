// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAttackNS.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API UMeleeAttackNS : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation) override;
};
