// Fill out your copyright notice in the Description page of Project Settings.


#include "CenterActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ACenterActor::ACenterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComp;

	SphereComp->InitSphereRadius(40.f);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

}

// Called when the game starts or when spawned
void ACenterActor::BeginPlay()
{
	Super::BeginPlay();
	
}
