// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AITEnemyAIController.h"
#include "AI/AITEnemyAICharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAITEnemyAIController::AAITEnemyAIController(const FObjectInitializer& Initializer) : Super(Initializer)
{
	bStartAILogicOnPossess = true;

	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
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
			if (UBehaviorTreeComponent* BT = Cast<UBehaviorTreeComponent>(BrainComponent))
			{
				BT->StartTree(*AIChar->BehaviorTree);
			}
		} 
		else
		{
		}

		if (AIChar->PerceptionComp)
		{
			PerceptionComponent = AIChar->PerceptionComp;
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAITEnemyAIController::OnPerceptionUpdated);
		}
	}

	Super::OnPossess(InPawn);
}

void AAITEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("AAITEnemyAIController::OnPerceptionUpdated() - %s for target %s"), *Stimulus.GetDebugDescription(), *Actor->GetName());
}