// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PetFollower.generated.h"

UCLASS()
class RPGZELDA_API APetFollower : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APetFollower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pet Settings")
	float SpeedMovement = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pet Settings")
	float MinimumDistanceToActor = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pet Settings")
	float StopRadius = 50.f;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Pet Settings")
	USceneComponent* ActorToFollow;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pet Settings")
		void StartFollow(USceneComponent* Actor);

	void StartFollow_Implementation(USceneComponent* Actor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pet Settings")
		void StopFollow();

	void StopFollow_Implementation();

protected:

	int32 CurrentState;

	FVector LastLocation;
};
