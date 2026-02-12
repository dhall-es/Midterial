// Fill out your copyright notice in the Description page of Project Settings.


#include "MidterialSlateBPLibrary.h"

void UMidterialSlateBPLibrary::LoadMidterialMIWindow(TArray<UObject*> Textures)
{
	TSharedRef<SMidterialWidgetMI> Widget = SNew(SMidterialWidgetMI);
	for (auto& oTex : Textures)
	{
		if (oTex->IsA(UTexture::StaticClass()) == false)
		{
			continue;
		}

		Widget->AddListItem(MakeShareable(new FString(oTex->GetPathName())));
	}

	TSharedRef<SWindow> Window = SNew(SWindow)
		.MinWidth(400.0f)
		.MinHeight(200.0f)
		.IsTopmostWindow(true)
		.Title(FText::FromString("Build Material Instance"));

	Window->SetContent(Widget);
	FSlateApplication::Get().AddWindow(Window);
}

void UMidterialSlateBPLibrary::LoadMidterialMMWindow(TArray<UObject*> Textures)
{
	TSharedRef<SMidterialWidgetMM> Widget = SNew(SMidterialWidgetMM);

	TSharedRef<SWindow> Window = SNew(SWindow)
		.MinWidth(400.0f)
		.MinHeight(200.0f)
		.IsTopmostWindow(true)
		.Title(FText::FromString("Build Master Material"));

	Window->SetContent(Widget);
	FSlateApplication::Get().AddWindow(Window);
}
