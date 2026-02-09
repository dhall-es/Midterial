// Fill out your copyright notice in the Description page of Project Settings.


#include "MidterialSlateBPLibrary.h"

void UMidterialSlateBPLibrary::ShowMidterialSlate()
{
	//TSharedRef<SVerticalBox> windowContent = SNew(SVerticalBox)
	//	+ SVerticalBox::Slot()
	//	[
	//		SNew(SAssetDropTarget)
	//	]
	//	+ SVerticalBox::Slot()
	//	[
	//		SNew(STextBlock)
	//			.Text(FText::FromString("Hello"))
	//	];

	//TSharedRef<SWindow> window = SNew(SWindow);
	//window->SetContent(windowContent);
	//FSlateApplication::Get().AddWindow(window);
	UE_LOG(LogTemp, Warning, TEXT("Received command Show Midterial Slate"))
	FMidterialModule::InvokeMidterialWindow();
}