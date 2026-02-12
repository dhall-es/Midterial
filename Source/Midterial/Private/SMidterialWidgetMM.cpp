// Fill out your copyright notice in the Description page of Project Settings.


#include "SMidterialWidgetMM.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMidterialWidgetMM::Construct(const FArguments& InArgs)
{	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.Padding(5.0f)
		[
			SNew(SExpandableArea)
			.AllowAnimatedTransition(false)
			.HeaderContent()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Base Color"))
			]
			.HeaderPadding(FMargin(5.0f, 2.0f))
			.Padding(FMargin(10.0f, 5.0f, 5.0f, 5.0f))
			.BodyContent()
			[
				SAssignNew(BaseColorSection, SMidPropertySection)
				.TextureName(TEXT("Base Color"))
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SButton)
			.OnClicked(FOnClicked::CreateSP(this, &SMidterialWidgetMM::OnBuildButtonClicked))
			.Text(FText::FromString("Build Master Material"))
			.HAlign(HAlign_Center)
		]
	];
}

FReply SMidterialWidgetMM::OnBuildButtonClicked()
{
	FString MaterialPath{};
	MaterialPath = TEXT("/Game/MM_Example.MM_Example");

	TArray<SMidPropertySection*> Sections{};

	Sections.Add(BaseColorSection.Get());

	UMidterialBPLibrary::BuildMasterMaterialSections(MaterialPath, Sections);

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
