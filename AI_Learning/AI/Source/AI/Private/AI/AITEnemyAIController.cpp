// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AITEnemyAIController.h"
#include "AI/AITEnemyAICharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

DEFINE_LOG_CATEGORY(LogEnemyAI);

FAISenseID AAITEnemyAIController::SightSenseID = {};
FAISenseID AAITEnemyAIController::HearingSenseID = {};

AAITEnemyAIController::AAITEnemyAIController(const FObjectInitializer& Initializer) : Super(Initializer), TeamID(2), EnemyTeamID(1)
{
	bStartAILogicOnPossess = true;

	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	TargetKey = TEXT("Target");
	LastSeenLocationKey = TEXT("LastSeenLocation");
	LastHeardLocationKey = TEXT("LastHeardLocation");
	WanderStartLocationKey = TEXT("WanderStartLocation");

	SetGenericTeamId(TeamID);

	SightSenseID   = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	HearingSenseID = UAISense::GetSenseID(UAISense_Hearing::StaticClass());
}

void AAITEnemyAIController::OnPossess(APawn* InPawn)
{
	if (InPawn != nullptr && !IsValid(InPawn))
	{
		return;
	}

	if (AAITEnemyAICharacter* AIChar = Cast<AAITEnemyAICharacter>(InPawn))
	{
		if (AIChar->BehaviorTree)
		{
			if (Blackboard && AIChar->BehaviorTree->GetBlackboardAsset())
			{
				Blackboard->InitializeBlackboard(*AIChar->BehaviorTree->GetBlackboardAsset());
				Blackboard->CacheBrainComponent(*BrainComponent);
			}
			if (UBehaviorTreeComponent* BT = Cast<UBehaviorTreeComponent>(BrainComponent))
			{
				BT->StartTree(*AIChar->BehaviorTree);
			}
		}

		if (PerceptionComponent)
		{
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAITEnemyAIController::OnPerceptionUpdated);
			PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AAITEnemyAIController::OnPerceptionForgotten);
			//PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AAITEnemyAIController::OnPerceptionInfoUpdate);
		}
	}

	// Do last so our AI setup above now gets activated
	Super::OnPossess(InPawn);
}

void AAITEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor && Stimulus.IsValid())
	{
		UE_CLOG(UE_GET_LOG_VERBOSITY(LogEnemyAI) >= ELogVerbosity::Display, LogEnemyAI, Log, TEXT("AAITEnemyAIController::OnPerceptionUpdated() - %s for target %s"), *Stimulus.GetDebugDescription(), *Actor->GetName());

		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				BB->SetValueAsObject(TargetKey, Actor);
				TargetSensedCharacter = Cast<ACharacter>(Actor);
			}
			else
			{
				if (Stimulus.Type == SightSenseID)
				{
					FVector targetLocation = Actor->GetActorLocation();
					CheckFallingLocation(targetLocation);

					BB->SetValueAsVector(LastSeenLocationKey, targetLocation);
					BB->SetValueAsObject(TargetKey, nullptr);
				}
				else if (Stimulus.Type == HearingSenseID)
				{
					BB->SetValueAsVector(LastHeardLocationKey, Actor->GetActorLocation());
				}
			}
		}
	}
	else
	{
		UE_LOG(LogEnemyAI, Warning, TEXT("AAITEnemyAIController::OnPerceptionUpdated() - %s for target %s NOT VALID"), *Stimulus.GetDebugDescription(), *Actor->GetName());
	}
}

void AAITEnemyAIController::OnPerceptionForgotten(AActor* Actor)
{
	UE_CLOG(UE_GET_LOG_VERBOSITY(LogEnemyAI) >= ELogVerbosity::Display, LogEnemyAI, Warning, TEXT("AAITEnemyAIController::OnPerceptionForgotten() - for target %s"), *Actor->GetName());

	if (Actor == TargetSensedCharacter)
	{
		TargetSensedCharacter = nullptr;
	
		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsObject(TargetKey, nullptr);
		}
	}
}

void AAITEnemyAIController::OnPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
// 	if (UBlackboardComponent* BB = GetBlackboardComponent())
// 	{
// 		if (TargetSensedCharacter.IsValid() && TargetSensedCharacter == UpdateInfo.Target)
// 		{
// 			UE_CLOG(UE_GET_LOG_VERBOSITY(LogEnemyAI) >= ELogVerbosity::Display, LogEnemyAI, Log, TEXT("AAITEnemyAIController::OnPerceptionInfoUpdate() - %s for target %s"), *UpdateInfo.Stimulus.GetDebugDescription(), *TargetSensedCharacter->GetName());
// 
// 			if (UpdateInfo.Stimulus.Type == SightSenseID)
// 			{
// 				FVector targetLocation = TargetSensedCharacter->GetActorLocation();
// 				CheckFallingLocation(targetLocation);
// 
// 				BB->SetValueAsVector(LastSeenLocationKey, targetLocation);
// 			}
// 			else if (UpdateInfo.Stimulus.Type == HearingSenseID)
// 			{
// 				BB->SetValueAsVector(LastHeardLocationKey, TargetSensedCharacter->GetActorLocation());
// 			}
// 		}
// 	}
}

void AAITEnemyAIController::CheckFallingLocation(FVector& InOutTargetLocation)
{
	// Project any in-air locations to the ground to be useful for navigation
	if (Cast<ACharacter>(TargetSensedCharacter)->GetMovementComponent()->IsFalling())
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys != nullptr)
		{
			static const FVector extent(100.f, 100.f, 500.f);
			FNavLocation projectedLocation;
			if (NavSys->ProjectPointToNavigation(InOutTargetLocation, projectedLocation, extent))
			{
				InOutTargetLocation = projectedLocation.Location;
			}
			else
			{
				InOutTargetLocation = FAISystem::InvalidLocation;
			}
		}
	}			
}

ETeamAttitude::Type AAITEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId otherID = TeamAgent->GetGenericTeamId();
			if (otherID == GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}
			else if (otherID == EnemyTeamID)
			{
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

bool AAITEnemyAIController::GetWanderStartLocation(FVector& OutWanderStartLoc)
{
	OutWanderStartLoc = FAISystem::InvalidLocation;

	FVector loc = GetBlackboardComponent()->GetValueAsVector(WanderStartLocationKey);
	if (loc != FAISystem::InvalidLocation)
	{
		OutWanderStartLoc = loc;
		return true;
	}
	else
	{
		if (GetPawn())
		{
			OutWanderStartLoc = GetPawn()->GetActorLocation();
		}
		return false;
	}
}
