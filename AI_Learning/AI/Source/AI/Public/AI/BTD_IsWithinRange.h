// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsWithinRange.generated.h"

/**
 * 
 */
UCLASS()
class AI_API UBTD_IsWithinRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_IsWithinRange();

	UPROPERTY(EditAnywhere, meta = (ClampMin="0.0"))
	float Range = 1000.f;;

	/** blackboard key Actor selector for start of range check. */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector StartActorKey;
	
	/** blackboard key Actor selector for end of range check. */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector EndActorKey;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
