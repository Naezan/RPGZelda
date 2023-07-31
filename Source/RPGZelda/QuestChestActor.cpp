// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestChestActor.h"
#include "InteractiveNPCCharacter.h"

#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "LinkCharacter.h"
#include "RPGPlayerController.h"
#include "IngameUI.h"

#include "Engine/StaticMeshActor.h"
#include "GameFramework/RotatingMovementComponent.h"

AQuestChestActor::AQuestChestActor()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	//스프링암 컴포넌트 생성
	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 150.f),
		FRotator(-10.f, 0.f, 0.f));
	CameraSpringArmComp->TargetArmLength = 150.f;
	CameraSpringArmComp->bEnableCameraLag = true;
	CameraSpringArmComp->CameraLagSpeed = 5.f;
	CameraSpringArmComp->SetupAttachment(MeshComp);

	//카메라 컴포넌트 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ChestCamera"));
	CameraComp->SetupAttachment(CameraSpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	PickUpPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpPoint"));
	PickUpPoint->SetupAttachment(MeshComp);

	ItemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawnPoint"));
	ItemSpawnPoint->SetupAttachment(MeshComp);

	bIsQuestComplete = false;
	MoveComplete = false;
}

bool AQuestChestActor::GetIsInteract()
{
	return bIsInteract;
}

void AQuestChestActor::SetIsInteract(bool bInteract)
{
	bIsInteract = bInteract;
}

void AQuestChestActor::SetMoveComplete(bool bComplete)
{
	MoveComplete = bComplete;
}

void AQuestChestActor::SetAlpha(float bAlpha)
{
	Alpha = bAlpha;
}

void AQuestChestActor::OnDialogueCompleted()
{
	if (CurrentPlayerController != nullptr)
	{
		//몽타주 되돌리기
		DialogueAnimEnd();

		//임시 객체 삭제함수
		if (ItemActor != nullptr)
			ItemActor->Destroy();

		//줌아웃
		if (PlayerCharacter != nullptr)
		{
			CurrentPlayerController->SetViewTargetWithBlend(
				CurrentPlayerController->GetPawn(), CameraBlendTime,
				EViewTargetBlendFunction::VTBlend_Linear);
		}

		//퀘스트아이템 추가
		CurrentPlayerController->GrantedItemChanged();
		CurrentPlayerController->ShowHUD(true);

		CurrentPlayerController->GetUI()->
			OnDialogueCompleted.RemoveDynamic(this, &AQuestChestActor::OnDialogueCompleted);
	}
}

void AQuestChestActor::InteractDialogue()
{
	if ((CurrentPlayerController != nullptr) && (Dialogue != nullptr))
	{
		CurrentPlayerController->GetUI()->InitializeDialogue(Dialogue);

		CurrentPlayerController->GetUI()->
			OnDialogueCompleted.AddDynamic(this, &AQuestChestActor::OnDialogueCompleted);
	}
}

void AQuestChestActor::ZoomInCamera()
{
	if (PlayerCharacter != nullptr)
	{
		CurrentPlayerController->SetViewTargetWithBlend(
			CameraComp->GetOwner(), CameraBlendTime,
			EViewTargetBlendFunction::VTBlend_Linear);
	}
}

void AQuestChestActor::MoveToPickUpPoint(AActor* MoveActor, float DeltaTime)
{
	if (MoveComplete)
		return;

	FVector MoveActorPosition = MoveActor->GetActorLocation();
	FVector PickUpLocation = PickUpPoint->GetComponentLocation();

	//픽업포인트에 가까우면 스피드 줄이기
	//float DistToPickUpPoint = FVector::Dist(PickUpLocation, MoveActorPosition);

	FVector MoveActorDirection = PickUpLocation - MoveActorPosition;
	MoveActorDirection.Z = 0;
	MoveActorDirection.GetSafeNormal();

	FRotator NewRotation = MoveActorDirection.Rotation();
	NewRotation.Pitch = 0;
	NewRotation.Roll = 0;

	FVector NewLocation = MoveActor->GetActorLocation();
	NewLocation += MoveActorDirection * Speed * DeltaTime;
	FRotator CurRotation = MoveActor->GetActorRotation();
	CurRotation = FMath::RInterpTo(CurRotation, NewRotation, DeltaTime, 5.f);

	MoveActor->SetActorLocationAndRotation(NewLocation, CurRotation, true);

	float DistToLastLocation = FVector::Dist(MoveActor->GetActorLocation(), LastLocation);

	float CurrentSpeed = DistToLastLocation / DeltaTime;

	if (CurrentSpeed < 10.f)
	{
		Speed = 0.f;
		MoveActor->SetActorLocationAndRotation(PickUpLocation, PickUpPoint->GetComponentRotation(), true);
		MoveComplete = true;
	}

	LastLocation = MoveActor->GetActorLocation();
}

void AQuestChestActor::SpawnStaticMeshActor(const FVector& InLocation)
{
	ItemActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
	ItemActor->SetMobility(EComponentMobility::Movable);
	ItemActor->SetActorLocation(InLocation);
	UStaticMeshComponent* MeshComponent = ItemActor->GetStaticMeshComponent();
	if (MeshComponent)
	{
		MeshComponent->SetStaticMesh(ItemMesh);
	}

	//회전 컴포넌트 추가
	URotatingMovementComponent* RotatingMoveComp = NewObject<URotatingMovementComponent>(ItemActor);
	RotatingMoveComp->RegisterComponent();
}

void AQuestChestActor::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bIsQuestComplete)
		return;

	//캐릭터가 움직이기 시작할 처음 위치
	LastLocation = InstigatorPawn->GetActorLocation();

	//플레이어 컨트롤러 찾기
	PlayerCharacter = Cast<ALinkCharacter>(InstigatorPawn);
	if (PlayerCharacter != nullptr)
	{
		CurrentPlayerController = Cast<ARPGPlayerController>(PlayerCharacter->GetController());
		CurrentPlayerController->ShowHUD(false);//HUD하이드
	}

	//퀘스트가 끝났다면 호출안함
	if (TargetNPC != nullptr)
	{
		bIsQuestComplete = true;
		TargetNPC->SetIsQuestDone(true);

		UE_LOG(LogTemp, Warning, TEXT("QuestComplete"));
	}
}
