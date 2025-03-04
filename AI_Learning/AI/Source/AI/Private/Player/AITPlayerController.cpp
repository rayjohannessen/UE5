// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AITPlayerController.h"

AAITPlayerController::AAITPlayerController() : Super(), TeamID(1)
{
}

FGenericTeamId AAITPlayerController::GetGenericTeamId() const
{
	return TeamID;
}
