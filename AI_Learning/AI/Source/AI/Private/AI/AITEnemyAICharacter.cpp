// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AITEnemyAICharacter.h"
#include "AI/AITEnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"

AAITEnemyAICharacter::AAITEnemyAICharacter()
{
	AIControllerClass = AAITEnemyAIController::StaticClass();

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
}
