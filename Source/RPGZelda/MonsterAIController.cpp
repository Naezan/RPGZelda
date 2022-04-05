// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "CharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const float AMonsterAIController::AI_SIGHT_RADIUS = 2500.f;
const float AMonsterAIController::AI_SIGHT_AGE = 5.f;
const float AMonsterAIController::AI_LOSE_SIGHT_RADIUS = AI_SIGHT_RADIUS + 100.f;
const float AMonsterAIController::AI_FIELD_OF_VIEW = 90.f;

const FName AMonsterAIController::TARGET_KEY = TEXT("TargetToFollow");
const FName AMonsterAIController::TARGET_LOCATION_KEY = TEXT("MoveToLocation");
const FName AMonsterAIController::TARGET_LENGTH_KEY = TEXT("DistToTarget");

AMonsterAIController::AMonsterAIController()
{
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AI_SIGHT_RADIUS;
	SightConfig->LoseSightRadius = AI_LOSE_SIGHT_RADIUS;
	SightConfig->PeripheralVisionAngleDegrees = AI_FIELD_OF_VIEW;
	SightConfig->SetMaxAge(AI_SIGHT_AGE);

	//감시대상 셋팅
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	DamageConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	GetPerceptionComponent()->ConfigureSense(*DamageConfig);

	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "OnBaseTargetPerceptionUpdate");
	GetPerceptionComponent()->OnTargetPerceptionUpdated.Add(Delegate);

	//적인지 아군인지 할당
	SetGenericTeamId(FGenericTeamId(5));
}

//액터가 초기화된 후 호출
void AMonsterAIController::PostInitializeComponents()
{
	//AI가 플레이어의 상태를 원하는지 여부
	bWantsPlayerState = true;
	Super::PostInitializeComponents();
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//컨트롤러가 소유하고있고 블랙보드를 사용중이라면 행동트리 실행
	if (UseBlackboard(BBComp, Blackboard))
	{
		RunBehaviorTree(BTComp);
	}
}

//델리게이트 호출
void AMonsterAIController::OnBaseTargetPerceptionUpdate(AActor* actor, FAIStimulus stimulus)
{
	ACharacterBase* CharacterBase = Cast<ACharacterBase>(actor);
	if (CharacterBase)
	{
		if (CharacterBase->IsAlive())
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), actor);
		}
	}
}

ETeamAttitude::Type AMonsterAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			//Create an alliance with Team with ID 10 and set all the other teams as Hostiles:
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID == 5) {
				return ETeamAttitude::Friendly;
			}
			else {
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

AActor* AMonsterAIController::GetSeeingPawn()
{
	//플레이어 찾기
	UObject* object = GetBlackboardComponent()->GetValueAsObject(TARGET_KEY);

	return object ? Cast<AActor>(object) : nullptr;
}
