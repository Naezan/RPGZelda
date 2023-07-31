// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossAIController.h"
#include "BlaazSplitAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGZELDA_API ABlaazSplitAIController : public ABossAIController
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ABlaazSplitAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Center")
		AActor* Center;
};
