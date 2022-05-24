// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APatrolPathActor::APatrolPathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComp;

	SphereComp->InitSphereRadius(40.f);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void APatrolPathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector APatrolPathActor::GetPatrolPoint(int index) const
{
	return PatrolPoints[index];
}

int APatrolPathActor::GetPatrolPointNum() const
{
	return PatrolPoints.Num();
}

