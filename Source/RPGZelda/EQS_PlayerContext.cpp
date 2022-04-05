// Fill out your copyright notice in the Description page of Project Settings.


#include "EQS_PlayerContext.h"
#include "MonsterAIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"



void UEQS_PlayerContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//Find AIController and Find Player
	AMonsterAIController* AICon = Cast<AMonsterAIController>(
		(Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	if (AICon && AICon->GetSeeingPawn())
	{
		//Set context SeeingPawn(player maybe) to Contextdata
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, AICon->GetSeeingPawn());
	}
}
