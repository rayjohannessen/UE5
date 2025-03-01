//

#include "AI/AITSpawnVolume.h"
#include "AI/AITEnemyAISubsystem.h"
#include "Game/AITGameMode.h"

AAITSpawnVolume::AAITSpawnVolume()
{
	OnActorBeginOverlap.AddDynamic(this, &AAITSpawnVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AAITSpawnVolume::OnOverlapEnd);
}

void AAITSpawnVolume::BeginPlay()
{
	AAITGameMode* gameMode = GetWorld()->GetAuthGameMode<AAITGameMode>();
	if (gameMode)
	{
		gameMode->EnemyManager->RegisterSpawnVolume(this);
	}
}

void AAITSpawnVolume::EndPlay(const EEndPlayReason::Type Reason)
{
	AAITGameMode* gameMode = GetWorld()->GetAuthGameMode<AAITGameMode>();
	if (gameMode)
	{
		gameMode->EnemyManager->UnregisterSpawnVolume(this);
	}
}

void AAITSpawnVolume::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{

}

void AAITSpawnVolume::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{

}

bool AAITSpawnVolume::CanUseToSpawn(const TSubclassOf<AAITBaseCharacter>& DesiredSpawnClasss)
{
	return true;
}
