// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "AITEnemyAIController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEnemyAI, Display, All);

struct FActorPerceptionUpdateInfo;

/**
 * 
 */
UCLASS()
class AI_API AAITEnemyAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
	
public:

	AAITEnemyAIController(const FObjectInitializer& Initializer);

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName LastSeenLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName LastHeardLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName WanderStartLocationKey;

	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId TeamID;

	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId EnemyTeamID;

protected:

	static FAISenseID SightSenseID;
	static FAISenseID HearingSenseID;

	TWeakObjectPtr<ACharacter> TargetSensedCharacter;

protected:

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnPerceptionForgotten(AActor* Actor);

	UFUNCTION()
	void OnPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo);

private:

	void CheckFallingLocation(FVector& InOutTargetLocation);

public:

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// Sets OutWanderStartLoc to WanderStartLocation if valid, the pawn's location otherwise
	bool GetWanderStartLocation(FVector& OutWanderStartLoc);
};
