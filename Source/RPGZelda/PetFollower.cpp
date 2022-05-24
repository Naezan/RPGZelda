// Fill out your copyright notice in the Description page of Project Settings.


#include "PetFollower.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LinkCharacter.h"

// Sets default values
APetFollower::APetFollower()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APetFollower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	CurrentState = 0;

	LastLocation = GetActorLocation();
}

// Called every frame
void APetFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector CurrentLocation = GetActorLocation();

	FVector OtherActorPosition = ActorToFollow->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Y : %f"), OtherActorPosition.Y);
	//UE_LOG(LogTemp, Warning, TEXT("Z : %f"), OtherActorPosition.Z);
	//UE_LOG(LogTemp, Warning, TEXT("State : %d"), CurrentState);

	float DistanceToActor = FVector::Dist(OtherActorPosition, CurrentLocation);

	if (CurrentState == 1)
	{
		float Speed = SpeedMovement;
		if (DistanceToActor < StopRadius)
		{
			float SpeedFactor = UKismetMathLibrary::NormalizeToRange(
				DistanceToActor, MinimumDistanceToActor, StopRadius);
			Speed *= SpeedFactor;
		}
		if (Speed > 5.f)
		{
			FVector DirectionToFollow = OtherActorPosition - CurrentLocation;
			DirectionToFollow = DirectionToFollow.GetSafeNormal();

			FRotator TargetRotation = DirectionToFollow.Rotation();
			TargetRotation.Pitch = 0.f;
			TargetRotation.Roll = 0.f;

			FVector NewLocation = GetActorLocation();
			NewLocation += DirectionToFollow * Speed * DeltaTime;
			FRotator CurrentRotation = GetActorRotation();
			CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.f);

			SetActorLocationAndRotation(NewLocation, CurrentRotation, true);

			float DistanceToLastLoc = FVector::Dist(GetActorLocation(), LastLocation);

			float CurrentSpeed = DistanceToLastLoc / DeltaTime;

			if (CurrentSpeed < 20.f)
			{
				Speed = 0.f;
				CurrentState = 2;
			}
		}
	}
	else if (CurrentState == 2)
	{
		if (DistanceToActor >= StopRadius)
		{
			CurrentState = 1;
		}
	}

	LastLocation = GetActorLocation();
}

void APetFollower::StartFollow_Implementation(USceneComponent* Actor)
{
	ActorToFollow = Actor;

	if (ActorToFollow != nullptr)
	{
		CurrentState = 2;
		SetActorTickEnabled(true);
	}
}

void APetFollower::StopFollow_Implementation()
{
	CurrentState = 0;

	ActorToFollow = nullptr;

	SetActorTickEnabled(false);
}
