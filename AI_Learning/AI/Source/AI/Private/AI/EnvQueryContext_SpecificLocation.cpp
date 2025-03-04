// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnvQueryContext_SpecificLocation.h"
#include "AI/AITEnemyAIController.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "AITypes.h"

void UEnvQueryContext_SpecificLocation::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if (APawn* QuerierPawn = Cast<APawn>(QueryInstance.Owner.Get()))
	{
		if (AAITEnemyAIController* Controller = Cast<AAITEnemyAIController>(QuerierPawn->GetController()))
		{
			FVector ResultingLocation = FAISystem::InvalidLocation;
			Controller->GetWanderStartLocation(ResultingLocation);
			UEnvQueryItemType_Point::SetContextHelper(ContextData, ResultingLocation);
		}
	}
}