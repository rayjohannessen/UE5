// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISubsystem.h"

#include "AITEnemyAISubsystem.generated.h"

class AAITSpawnVolume;
class AAITBaseCharacter;
struct FAISpawnSettings;

struct FSpawnDataRuntime
{
	bool	bIsActive = false;
	float	SpawnTimestamp = 0.f;
	TSubclassOf<AAITBaseCharacter> ClassToSpawn = nullptr;
	const FAISpawnSettings* SpawnSettings = nullptr;

	TSet<AActor*> SpawnedActors;
};

/**
 * 
 */
UCLASS()
class AI_API UAITEnemyAISubsystem : public UAISubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize();
	void StartSystem();

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY()
	TObjectPtr<class UAITSystem> AITSystem = nullptr;

	UPROPERTY()
	TSet<AAITSpawnVolume*> AvailableSpawns;

	TArray<FSpawnDataRuntime> RuntimeSpawnData;

	void SpawnIntervalSatisfied(FSpawnDataRuntime& ForSpawnData, const float InTimestamp);

	UFUNCTION()
	void OnActorDestroyed(AActor* DestroyedActor);

public:
	void RegisterSpawnVolume(AAITSpawnVolume* NewSpawnVolume);
	void UnregisterSpawnVolume(AAITSpawnVolume* SpawnVolume);
};
