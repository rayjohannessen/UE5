// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIGameMode.h"
#include "AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EnemyAISubsystem.h"

AAIGameMode::AAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAIGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	EnemyManager = NewObject<UEnemyAISubsystem>(GetWorld());
	if (!ensure(EnemyManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIGameMode::InitGame() - Enemy Manager is null")); 
	}
	else
	{
		EnemyManager->Initialize();
	}
}

void AAIGameMode::StartPlay()
{
	Super::StartPlay();

	if (ensure(EnemyManager))
	{
		EnemyManager->Initialize();
	}
}
