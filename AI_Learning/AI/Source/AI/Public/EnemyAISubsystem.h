// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISubsystem.h"
#include "EnemyAISubsystem.generated.h"

class AAISpawnVolume;
class AAICharacter;
struct FAISpawnSettings;

struct FSpawnDataRuntime
{
	bool	bIsActive = false;
	int32	CurrentCount = 0;
	float	SpawnTimestamp = 0.f;
	TSubclassOf<AAICharacter> ClassToSpawn = nullptr;
	const FAISpawnSettings* SpawnSettings = nullptr;
};

/**
 * 
 */
UCLASS()
class AI_API UEnemyAISubsystem : public UAISubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize();
	void StartSystem();

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY()
	TObjectPtr<class UMyAISystem> MyAISys = nullptr;

	UPROPERTY()
	TSet<AAISpawnVolume*> AvailableSpawns;

	TArray<FSpawnDataRuntime> SpawnData;

	void SpawnIntervalSatisfied(FSpawnDataRuntime& ForSpawnData, const float InTimestamp);

public:
	void RegisterSpawnVolume(AAISpawnVolume* NewSpawnVolume);
	void UnregisterSpawnVolume(AAISpawnVolume* SpawnVolume);
};
