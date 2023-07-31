// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItemInterface.h"
#include "InteractiveActorBase.generated.h"

UCLASS()
class RPGZELDA_API AInteractiveActorBase : public AActor, public IInteractiveItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActorBase();

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	//it doesn't have collision, only use meshcollision
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp;

};
