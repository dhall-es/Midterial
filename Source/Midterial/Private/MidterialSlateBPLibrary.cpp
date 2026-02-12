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
	FString MaterialName{}, BaseColorPath{}, NormalPath{}, ORMPath{};

	for (auto& oTex : Textures)
	{
		FString Name{};

		oTex->GetPathName().Split(TEXT("."), nullptr, &Name);

		if (Name.IsEmpty())
		{
			continue;
		}
		if (MaterialName.IsEmpty())
		{
			FString NoExtension{};

			if (Name.Split(TEXT("_"), &NoExtension, nullptr))
			{
				MaterialName = TEXT("MM_") + NoExtension;
			}
			else
			{
				MaterialName = TEXT("MM_") + Name;
			}
		}

		if (Name.Contains(TEXT("Normal"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			NormalPath = oTex->GetPathName();
			continue;
		}
		if (Name.Contains(TEXT("Color"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			BaseColorPath = oTex->GetPathName();
			continue;
		}
		if (Name.Contains(TEXT("ORM"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			ORMPath = oTex->GetPathName();
			continue;
		}
	}

	TSharedRef<SMidterialWidgetMM> Widget = SNew(SMidterialWidgetMM)
		.MaterialName(MaterialName)
		.BaseColorPath(BaseColorPath)
		.NormalPath(NormalPath)
		.ORMPath(ORMPath);

	TSharedRef<SWindow> Window = SNew(SWindow)
		.MinWidth(200.0f)
		.MinHeight(400.0f)
		.IsTopmostWindow(true)
		.Title(FText::FromString("Build Master Material"));

	Window->SetContent(Widget);
	FSlateApplication::Get().AddWindow(Window);
}
