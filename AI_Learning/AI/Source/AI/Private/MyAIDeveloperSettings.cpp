// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIDeveloperSettings.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "MyAIEngineSettings"

FName UMyAIDeveloperSettings::GetCategoryName() const
{
	return TEXT("Engine");
}

#if WITH_EDITOR
FText UMyAIDeveloperSettings::GetSectionText() const
{
	return LOCTEXT("MyAISystemSettingsName", "My AI System");

}
#endif

#undef LOCTEXT_NAMESPACE