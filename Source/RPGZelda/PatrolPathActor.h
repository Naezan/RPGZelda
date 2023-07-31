// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPathActor.generated.h"

UCLASS()
class RPGZELDA_API APatrolPathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FVector GetPatrolPoint(int index) const;
	int GetPatrolPointNum() const;

protected:
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrol, meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector> PatrolPoints;
};
