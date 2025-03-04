// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ClearBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_ClearBlackboardKey::UBTTask_ClearBlackboardKey()
{
	NodeName = TEXT("Clear BB Key");
}

EBTNodeResult::Type UBTTask_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type result = EBTNodeResult::Succeeded;

	if (GetSelectedBlackboardKey() != NAME_None)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
		} 
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FAISystem::InvalidLocation);
		}
		else
		{
			ensure(false);
			result = EBTNodeResult::Failed;
		}
	}

	return result;
}

FString UBTTask_ClearBlackboardKey::GetStaticDescription() const
{
	return FString::Printf(TEXT("Clear Blackboard Key '%s'"), *GetSelectedBlackboardKey().ToString());
}
