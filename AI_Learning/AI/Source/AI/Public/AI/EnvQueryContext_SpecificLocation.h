// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Item.h"
#include "EnvQueryContext_SpecificLocation.generated.h"

/**
 * 
 */
UCLASS()
class AI_API UEnvQueryContext_SpecificLocation : public UEnvQueryContext_Item
{
	GENERATED_BODY()
	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
