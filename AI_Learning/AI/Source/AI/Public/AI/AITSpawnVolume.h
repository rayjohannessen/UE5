
#pragma once

#include "Engine/TriggerVolume.h"

#include "AITBaseCharacter.h"
#include "AITSpawnVolume.generated.h"

UCLASS()
class AAITSpawnVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	AAITSpawnVolume();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	bool CanUseToSpawn(const TSubclassOf<AAITBaseCharacter>& DesiredSpawnClasss);
};