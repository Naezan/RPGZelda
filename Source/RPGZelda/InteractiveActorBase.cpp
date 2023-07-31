// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActorBase.h"

// Sets default values
AInteractiveActorBase::AInteractiveActorBase()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	RootComponent = SceneComp;
}

void AInteractiveActorBase::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}

