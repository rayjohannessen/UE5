
#pragma once

#include "Engine/TriggerVolume.h"

#include "AI/AICharacter.h"
#include "AISpawnVolume.generated.h"

UCLASS()
class AAISpawnVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	AAISpawnVolume();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	bool CanUseToSpawn(const TSubclassOf<AAICharacter>& DesiredSpawnClasss);
};