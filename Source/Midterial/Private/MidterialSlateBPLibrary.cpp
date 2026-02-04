// Fill out your copyright notice in the Description page of Project Settings.


#include "MidterialSlateBPLibrary.h"

void UMidterialSlateBPLibrary::ShowMidterialSlate()
{
	TSharedRef<SVerticalBox> windowContent = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(SImage)
		]
		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
				.Text(FText::FromString("Hello"))
		];

	TSharedRef<SWindow> window = SNew(SWindow);
	window->SetContent(windowContent);
	FSlateApplication::Get().AddWindow(window);
}