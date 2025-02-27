// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAICharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

AEnemyAICharacter::AEnemyAICharacter()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}
