// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "AITEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AI_API AAITEnemyAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
	
public:

	AAITEnemyAIController(const FObjectInitializer& Initializer);

	virtual void OnPossess(APawn* InPawn) override;

protected:

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
