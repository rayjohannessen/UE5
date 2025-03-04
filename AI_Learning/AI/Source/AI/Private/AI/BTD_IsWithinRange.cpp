// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD_IsWithinRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTD_IsWithinRange::UBTD_IsWithinRange()
{
	NodeName = TEXT("Is Within Range");

	StartActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsWithinRange, StartActorKey), AActor::StaticClass());
	EndActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsWithinRange, EndActorKey), AActor::StaticClass());

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	FlowAbortMode = EBTFlowAbortMode::None;
}

void UBTD_IsWithinRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBData = GetBlackboardAsset())
	{
		StartActorKey.ResolveSelectedKey(*BBData);
		EndActorKey.ResolveSelectedKey(*BBData);
	}
}

bool UBTD_IsWithinRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (const UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent())
	{
		FVector startLocation = FAISystem::InvalidLocation;
		FVector endLocation = FAISystem::InvalidLocation;
		BBComponent->GetLocationFromEntry(StartActorKey.GetSelectedKeyID(), startLocation);
		BBComponent->GetLocationFromEntry(EndActorKey.GetSelectedKeyID(), endLocation);

		if (startLocation != FAISystem::InvalidLocation && endLocation != FAISystem::InvalidLocation)
		{
			float distSq = FVector::DistSquared(startLocation, endLocation);
			return distSq < FMath::Square(Range);
		}
	}

	return false;
}

FString UBTD_IsWithinRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s within %.2f of %s")
		, *Super::GetStaticDescription()
		, *StartActorKey.SelectedKeyName.ToString()
		, Range
		, *EndActorKey.SelectedKeyName.ToString());
}
