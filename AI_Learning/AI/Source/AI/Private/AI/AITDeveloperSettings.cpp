// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AITDeveloperSettings.h"

#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "AITEngineSettings"

FName UAITDeveloperSettings::GetCategoryName() const
{
	return TEXT("Engine");
}

#if WITH_EDITOR
FText UAITDeveloperSettings::GetSectionText() const
{
	return LOCTEXT("AITSystemSettingsName", "AIT System");

}
#endif

#undef LOCTEXT_NAMESPACE