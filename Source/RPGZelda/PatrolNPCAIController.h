// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCAIController.h"
#include "PatrolNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API APatrolNPCAIController : public ANPCAIController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APatrolNPCAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Patrol)
	AActor* Center;
};
