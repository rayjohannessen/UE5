// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "EnemyAICharacter.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UBlackboardData;

/**
 * 
 */
UCLASS()
class AI_API AEnemyAICharacter : public AAICharacter
{
	GENERATED_BODY()
	
public:

	AEnemyAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	TObjectPtr<UBlackboardComponent> BlackboardComp = nullptr;
};
