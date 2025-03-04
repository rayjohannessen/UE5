// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "AITPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AI_API AAITPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:

	AAITPlayerController();

	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId TeamID;

	virtual FGenericTeamId GetGenericTeamId() const override;
};
