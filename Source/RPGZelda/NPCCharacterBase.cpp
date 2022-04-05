// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacterBase.h"
#include "Components/BoxComponent.h"
#include "LinkCharacter.h"
#include "RPGPlayerController.h"

// Sets default values
ANPCCharacterBase::ANPCCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LookAtPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LookAtPoint"));
	LookAtPoint->SetupAttachment(RootComponent);

	InteractiveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveCollision"));
	InteractiveCollision->SetupAttachment(RootComponent);

	InteractiveCollision->OnComponentBeginOverlap.AddDynamic(
		this, &ANPCCharacterBase::OnOverlapBegin);
	InteractiveCollision->OnComponentEndOverlap.AddDynamic(
		this, &ANPCCharacterBase::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	bIsEnabled = true;
}

// Called every frame
void ANPCCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCCharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsEnabled) return;

	CurrentCharacter = Cast<ALinkCharacter>(OtherActor);

	if (CurrentCharacter)
	{
		CurrentPlayerController = Cast<ARPGPlayerController>(CurrentCharacter->GetController());

		CurrentCharacter->SetInteractiveInRange(this);
	}
}

void ANPCCharacterBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsEnabled) return;

	CurrentCharacter = Cast<ALinkCharacter>(OtherActor);

	if (CurrentCharacter)
	{
		CurrentCharacter->ClearInteractiveInRange(this);

		CurrentPlayerController = nullptr;

		//ÁÜ¾Æ¿ô?
	}
}

void ANPCCharacterBase::SetEnable(bool Enable)
{
	bIsEnabled = Enable;

	if (bIsEnabled)
	{
		InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		InteractiveCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ANPCCharacterBase::Interact()
{
	if (!bIsEnabled) return;
}

