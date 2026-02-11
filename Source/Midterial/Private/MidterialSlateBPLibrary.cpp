// Fill out your copyright notice in the Description page of Project Settings.


#include "MidterialSlateBPLibrary.h"

void UMidterialSlateBPLibrary::LoadMidterialMIWindow(TArray<UObject*> Textures)
{
	TSharedRef<SMidterialWidgetMI> widget = SNew(SMidterialWidgetMI);
	for (auto& oTex : Textures)
	{
		if (oTex->IsA(UTexture::StaticClass()) == false)
		{
			continue;
		}

		widget->AddListItem(MakeShareable(new FString(oTex->GetPathName())));
	}

	TSharedRef<SWindow> window = SNew(SWindow)
		.MinWidth(400.0f)
		.MinHeight(200.0f)
		.IsTopmostWindow(true)
		.Title(FText::FromString("Auto-Create Material Instance"));

	window->SetContent(widget);
	FSlateApplication::Get().AddWindow(window);
}
