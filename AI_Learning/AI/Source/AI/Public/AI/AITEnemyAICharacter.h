// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITBaseCharacter.h"
#include "AITEnemyAICharacter.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class AI_API AAITEnemyAICharacter : public AAITBaseCharacter
{
	GENERATED_BODY()
	
public:

	AAITEnemyAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;
};
