// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "AITDeveloperSettings.generated.h"

class AAITBaseCharacter;

USTRUCT()
struct FAISpawnSettings
{
	GENERATED_BODY()

	// Set to false to delay first spawn until SpawnInterval seconds have passed
	UPROPERTY(Config, EditDefaultsOnly, Category = "Spawning")
	bool bBeginSpawnOnStart = true;

	// Don't stop spawning until this count is reached
	UPROPERTY(Config, EditDefaultsOnly, Category = "Spawning")
	uint32 TargetCount = 1;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Spawning")
	float SpawnInterval = 5.0f;
};

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class AI_API UAITDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Spawning")
	int32 MaxAICount = 50;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Spawning")
	TMap<TSubclassOf<AAITBaseCharacter>, FAISpawnSettings> SpawnSettings;

	// Begin UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif // WITH_EDITOR
	// End UDeveloperSettings Interface

};
