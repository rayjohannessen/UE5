// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_SetBlackboardKey::UBTTask_SetBlackboardKey()
{
	NodeName = TEXT("Set BB Key");
}

void UBTTask_SetBlackboardKey::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		SourceKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_SetBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type result = EBTNodeResult::Succeeded;

	if (GetSelectedBlackboardKey() != NAME_None)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() &&
			SourceKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			FVector SourceLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(SourceKey.SelectedKeyName);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SourceLocation);
		}
		else
		{
			ensure(false);
			result = EBTNodeResult::Failed;
		}
	}

	return result;
}

FString UBTTask_SetBlackboardKey::GetStaticDescription() const
{
	return FString::Printf(TEXT("Set Blackboard Key '%s' to '%s'"), *GetSelectedBlackboardKey().ToString(), *SourceKey.SelectedKeyName.ToString());
}
