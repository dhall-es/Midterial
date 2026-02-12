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
			SNew(SScrollBox)
			+SScrollBox::Slot()
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
					SAssignNew(BaseColorSection, SBaseColorPropertySection)
				]
			]
			+SScrollBox::Slot()
			.Padding(5.0f)
			[
				SNew(SExpandableArea)
				.AllowAnimatedTransition(false)
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Normal"))
				]
				.HeaderPadding(FMargin(5.0f, 2.0f))
				.Padding(FMargin(10.0f, 5.0f, 5.0f, 5.0f))
				.BodyContent()
				[
					SAssignNew(NormalSection, SNormalPropertySection)
				]
			]
			+SScrollBox::Slot()
			.Padding(5.0f)
			[
				SNew(SExpandableArea)
				.AllowAnimatedTransition(false)
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(FText::FromString("ORM"))
				]
				.HeaderPadding(FMargin(5.0f, 2.0f))
				.Padding(FMargin(10.0f, 5.0f, 5.0f, 5.0f))
				.BodyContent()
				[
					SAssignNew(ORMSection, SORMPropertySection)
				]
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

FReply SMidterialWidgetMM::OnBuildButtonClicked() const
{
	FString MaterialPath{};
	MaterialPath = TEXT("/Game/MM_Example");

	TArray<SMidPropertySection*> Sections{};

	Sections.Add(BaseColorSection.Get());
	Sections.Add(NormalSection.Get());
	Sections.Add(ORMSection.Get());

	UMidterialBPLibrary::BuildMasterMaterialSections(MaterialPath, Sections);

	TArray<UObject*> AssetsToSync{};
	AssetsToSync.Add(StaticLoadObject(UObject::StaticClass(), nullptr, *MaterialPath));

	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync);

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
