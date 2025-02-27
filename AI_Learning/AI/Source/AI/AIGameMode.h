// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIGameMode.generated.h"

UCLASS(minimalapi)
class AAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAIGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	TObjectPtr<class UEnemyAISubsystem> EnemyManager = nullptr;
};

