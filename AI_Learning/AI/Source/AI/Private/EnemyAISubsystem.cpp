// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAISubsystem.h"
#include "MyAISystem.h"
#include "AISpawnVolume.h"
#include "EnemyAICharacter.h"
#include "MyAIDeveloperSettings.h"

#include "Kismet/GameplayStatics.h"

void UEnemyAISubsystem::Initialize()
{
	MyAISys = Cast<UMyAISystem>(GetOuter());
	if (const UMyAIDeveloperSettings* Settings = GetDefault<UMyAIDeveloperSettings>())
	{
		for (const TPair<TSubclassOf<AAICharacter>, FAISpawnSettings>& AITypeToSpawnSettings : Settings->SpawnSettings)
		{
			if (AITypeToSpawnSettings.Key && AITypeToSpawnSettings.Value.TargetCount > 0)
			{
				FSpawnDataRuntime runtimeData;
				runtimeData.ClassToSpawn = AITypeToSpawnSettings.Key;
				runtimeData.SpawnSettings = &AITypeToSpawnSettings.Value;
				SpawnData.Add(runtimeData);
			}
		}
	}
}

void UEnemyAISubsystem::StartSystem()
{
	const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	for (FSpawnDataRuntime& runtimeData : SpawnData)
	{
		runtimeData.bIsActive = true;
		if (runtimeData.SpawnSettings->bBeginSpawnOnStart)
		{
			SpawnIntervalSatisfied(runtimeData, CurrentTime);
		}
	}
}

void UEnemyAISubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	for (FSpawnDataRuntime& runtimeData : SpawnData)
	{
		if (runtimeData.bIsActive)
		{
			const float interval = CurrentTime - runtimeData.SpawnTimestamp;
			if (interval > runtimeData.SpawnSettings->SpawnInterval)
			{
				SpawnIntervalSatisfied(runtimeData, CurrentTime);
			}
		}
	}
}

void UEnemyAISubsystem::SpawnIntervalSatisfied(FSpawnDataRuntime& ForSpawnData, const float InTimestamp)
{
	AActor* spawnedActor = nullptr;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (AAISpawnVolume* spawnVolume : AvailableSpawns)
	{
		if (spawnVolume && spawnVolume->CanUseToSpawn(*ForSpawnData.ClassToSpawn))
		{
			spawnedActor = GetWorld()->SpawnActor<AEnemyAICharacter>(ForSpawnData.ClassToSpawn, spawnVolume->GetActorLocation(), spawnVolume->GetActorRotation(), spawnParams);
			break;
		}
	}

	if (spawnedActor)
	{
		ForSpawnData.SpawnTimestamp = InTimestamp;
	}
}


void UEnemyAISubsystem::RegisterSpawnVolume(AAISpawnVolume* NewSpawnVolume)
{
	AvailableSpawns.Add(NewSpawnVolume);
}

void UEnemyAISubsystem::UnregisterSpawnVolume(AAISpawnVolume* SpawnVolume)
{
	AvailableSpawns.Remove(SpawnVolume);
}