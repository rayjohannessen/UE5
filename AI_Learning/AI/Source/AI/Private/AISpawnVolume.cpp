
#include "AISpawnVolume.h"

#include "EnemyAISubsystem.h"
#include "AI/AIGameMode.h"

AAISpawnVolume::AAISpawnVolume()
{
	OnActorBeginOverlap.AddDynamic(this, &AAISpawnVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AAISpawnVolume::OnOverlapEnd);
}

void AAISpawnVolume::BeginPlay()
{
	AAIGameMode* gameMode = GetWorld()->GetAuthGameMode<AAIGameMode>();
	if (gameMode)
	{
		gameMode->EnemyManager->RegisterSpawnVolume(this);
	}
}

void AAISpawnVolume::EndPlay(const EEndPlayReason::Type Reason)
{
	AAIGameMode* gameMode = GetWorld()->GetAuthGameMode<AAIGameMode>();
	if (gameMode)
	{
		gameMode->EnemyManager->UnregisterSpawnVolume(this);
	}
}

void AAISpawnVolume::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{

}

void AAISpawnVolume::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{

}

bool AAISpawnVolume::CanUseToSpawn(const TSubclassOf<AAICharacter>& DesiredSpawnClasss)
{
	return true;
}
