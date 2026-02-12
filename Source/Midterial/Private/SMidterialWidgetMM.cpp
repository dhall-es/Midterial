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
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Material Name"))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(5.0f)
			[
				SNew(SEditableTextBox)
				.Justification(ETextJustify::Left)
				.Text(FText::FromString(InArgs._MaterialName))
				.OnTextChanged(FOnTextChanged::CreateSP(this, &SMidterialWidgetMM::OnNameChanged))
			]
		]
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
					.TexturePath(InArgs._BaseColorPath)
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
					.TexturePath(InArgs._NormalPath)
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
					.TexturePath(InArgs._ORMPath)
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
	if (Name.IsEmptyOrWhitespace())
	{
		MaterialPath = TEXT("/Game/MM_MidterialExample");
	}
	else
	{
		MaterialPath = TEXT("/Game/") + Name.ToString();
	}

	TArray<SMidPropertySection*> Sections{};

	Sections.Add(BaseColorSection.Get());
	Sections.Add(NormalSection.Get());
	Sections.Add(ORMSection.Get());

	BuildMasterMaterialSections(MaterialPath, Sections);

	TArray<UObject*> AssetsToSync{};
	AssetsToSync.Add(StaticLoadObject(UObject::StaticClass(), nullptr, *MaterialPath));

	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync);

	return FReply::Handled();
}

void SMidterialWidgetMM::OnNameChanged(const FText& NewName)
{
	Name = NewName;
}

void SMidterialWidgetMM::BuildMasterMaterialSections(FString MaterialPath, TArray<SMidPropertySection*> Sections)
{
	// Get material and texture
	UMaterial* Material = Cast<UMaterial>(StaticLoadObject(UObject::StaticClass(), nullptr, *MaterialPath));

	// Create material if it doesn't exist
	if (Material == nullptr)
	{
		bool bOutSuccess{};
		FString OutInfoMessage{};

		Material = UMidterialBPLibrary::CreateMaterialAsset(MaterialPath, bOutSuccess, OutInfoMessage);

		// If material creation fails, just return
		if (Material == nullptr)
		{
			return;
		}
	}

	for (auto& Section : Sections)
	{
		Section->AddExpressionsToMaterial(Material);
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
