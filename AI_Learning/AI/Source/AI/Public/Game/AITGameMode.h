// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AITGameMode.generated.h"

UCLASS(minimalapi)
class AAITGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAITGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UAITEnemyAISubsystem> EnemyManager = nullptr;
};

