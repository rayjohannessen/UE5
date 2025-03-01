// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AITEnemyAISubsystem.h"
#include "AI/AITSystem.h"
#include "AI/AITSpawnVolume.h"
#include "AI/AITEnemyAICharacter.h"
#include "AI/AITDeveloperSettings.h"

#include "Kismet/GameplayStatics.h"

void UAITEnemyAISubsystem::Initialize()
{
	//MyAISys = Cast<UMyAISystem>(GetOuter());
	if (const UAITDeveloperSettings* Settings = GetDefault<UAITDeveloperSettings>())
	{
		for (const TPair<TSubclassOf<AAITBaseCharacter>, FAISpawnSettings>& AITypeToSpawnSettings : Settings->SpawnSettings)
		{
			if (AITypeToSpawnSettings.Key && AITypeToSpawnSettings.Value.TargetCount > 0)
			{
				FSpawnDataRuntime runtimeData;
				runtimeData.ClassToSpawn = AITypeToSpawnSettings.Key;
				runtimeData.SpawnSettings = &AITypeToSpawnSettings.Value;
				RuntimeSpawnData.Add(runtimeData);
			}
		}
	}
}

void UAITEnemyAISubsystem::StartSystem()
{
	const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	for (FSpawnDataRuntime& runtimeData : RuntimeSpawnData)
	{
		runtimeData.bIsActive = true;
		if (runtimeData.SpawnSettings->bBeginSpawnOnStart)
		{
			SpawnIntervalSatisfied(runtimeData, CurrentTime);
		}
	}
}

void UAITEnemyAISubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	for (FSpawnDataRuntime& runtimeData : RuntimeSpawnData)
	{
		if (runtimeData.bIsActive && (uint32)runtimeData.SpawnedActors.Num() < runtimeData.SpawnSettings->TargetCount)
		{
			const float interval = CurrentTime - runtimeData.SpawnTimestamp;
			if (interval > runtimeData.SpawnSettings->SpawnInterval)
			{
				SpawnIntervalSatisfied(runtimeData, CurrentTime);
			}
		}
	}
}

void UAITEnemyAISubsystem::SpawnIntervalSatisfied(FSpawnDataRuntime& ForSpawnData, const float InTimestamp)
{
	AActor* spawnedActor = nullptr;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<AAITSpawnVolume*> availableSpawns = AvailableSpawns.Array();
	int32 numAvailable = availableSpawns.Num();
	int32 randIndex = FMath::RandHelper(numAvailable);

	while (numAvailable > 0)
	{
		AAITSpawnVolume* spawnVolume = availableSpawns[randIndex];
		if (spawnVolume && spawnVolume->CanUseToSpawn(*ForSpawnData.ClassToSpawn))
		{
			spawnedActor = GetWorld()->SpawnActor<AAITEnemyAICharacter>(ForSpawnData.ClassToSpawn, spawnVolume->GetActorLocation(), spawnVolume->GetActorRotation(), spawnParams);
			break;
		}

		--numAvailable;
		randIndex = (randIndex + 1) % numAvailable;
	}

	if (spawnedActor)
	{
		ForSpawnData.SpawnTimestamp = InTimestamp;
		ForSpawnData.SpawnedActors.Add(spawnedActor);
		spawnedActor->OnDestroyed.AddDynamic(this, &UAITEnemyAISubsystem::OnActorDestroyed);
	}
}

//////////////////////////////////////////////////////////////////////////

void UAITEnemyAISubsystem::OnActorDestroyed(AActor* DestroyedActor)
{
	for (FSpawnDataRuntime& runtimeData : RuntimeSpawnData)
	{
		if (runtimeData.SpawnedActors.Contains(DestroyedActor))
		{
			DestroyedActor->OnDestroyed.RemoveDynamic(this, &UAITEnemyAISubsystem::OnActorDestroyed);
			runtimeData.SpawnedActors.Remove(DestroyedActor);
			break;
		}
	}
}

void UAITEnemyAISubsystem::RegisterSpawnVolume(AAITSpawnVolume* NewSpawnVolume)
{
	AvailableSpawns.Add(NewSpawnVolume);
}

void UAITEnemyAISubsystem::UnregisterSpawnVolume(AAITSpawnVolume* SpawnVolume)
{
	AvailableSpawns.Remove(SpawnVolume);
}