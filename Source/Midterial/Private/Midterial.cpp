// Copyright Epic Games, Inc. All Rights Reserved.

#include "Midterial.h"

static const FName SlateMidterialWindowTabName("MidterialWindow");

#define LOCTEXT_NAMESPACE "FMidterialModule"

void FMidterialModule::StartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SlateMidterialWindowTabName, FOnSpawnTab::CreateRaw(this, &FMidterialModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSlateQuickstartWindowTabTitle", "MidterialWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMidterialModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SlateMidterialWindowTabName);
	
}

void FMidterialModule::InvokeMidterialWindow()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SlateMidterialWindowTabName);
}

TSharedRef<SDockTab> FMidterialModule::OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText(LOCTEXT("WindowWidgetText", "Placeholder"));

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SObjectPropertyEntryBox)
				]
		];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMidterialModule, Midterial)