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
				RuntimeSpawnData.Add(runtimeData);
			}
		}
	}
}

void UEnemyAISubsystem::StartSystem()
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

void UEnemyAISubsystem::Tick(float DeltaTime)
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

void UEnemyAISubsystem::SpawnIntervalSatisfied(FSpawnDataRuntime& ForSpawnData, const float InTimestamp)
{
	AActor* spawnedActor = nullptr;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<AAISpawnVolume*> availableSpawns = AvailableSpawns.Array();
	int32 numAvailable = availableSpawns.Num();
	int32 randIndex = FMath::RandHelper(numAvailable);

	while (numAvailable > 0)
	{
		AAISpawnVolume* spawnVolume = availableSpawns[randIndex];
		if (spawnVolume && spawnVolume->CanUseToSpawn(*ForSpawnData.ClassToSpawn))
		{
			spawnedActor = GetWorld()->SpawnActor<AEnemyAICharacter>(ForSpawnData.ClassToSpawn, spawnVolume->GetActorLocation(), spawnVolume->GetActorRotation(), spawnParams);
			break;
		}

		--numAvailable;
		randIndex = (randIndex + 1) % numAvailable;
	}

	if (spawnedActor)
	{
		ForSpawnData.SpawnTimestamp = InTimestamp;
		ForSpawnData.SpawnedActors.Add(spawnedActor);
		spawnedActor->OnDestroyed.AddDynamic(this, &UEnemyAISubsystem::OnActorDestroyed);
	}
}

//////////////////////////////////////////////////////////////////////////

void UEnemyAISubsystem::OnActorDestroyed(AActor* DestroyedActor)
{
	for (FSpawnDataRuntime& runtimeData : RuntimeSpawnData)
	{
		if (runtimeData.SpawnedActors.Contains(DestroyedActor))
		{
			DestroyedActor->OnDestroyed.RemoveDynamic(this, &UEnemyAISubsystem::OnActorDestroyed);
			runtimeData.SpawnedActors.Remove(DestroyedActor);
			break;
		}
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