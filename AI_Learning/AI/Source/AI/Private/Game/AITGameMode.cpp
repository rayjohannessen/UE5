// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/AITGameMode.h"
#include "AI/AITEnemyAISubsystem.h"

#include "UObject/ConstructorHelpers.h"

AAITGameMode::AAITGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAITGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	EnemyManager = NewObject<UAITEnemyAISubsystem>(GetWorld());
	if (!ensure(EnemyManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("AAITGameMode::InitGame() - Enemy Manager is null")); 
	}
	else
	{
		EnemyManager->Initialize();
	}
}

void AAITGameMode::StartPlay()
{
	Super::StartPlay();

	if (ensure(EnemyManager))
	{
		EnemyManager->StartSystem();
	}
}
