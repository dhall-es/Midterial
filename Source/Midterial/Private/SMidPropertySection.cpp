// Fill out your copyright notice in the Description page of Project Settings.


#include "SMidPropertySection.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMidPropertySection::Construct(const FArguments& InArgs)
{
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	bIsEnabledBoxChecked = true;

	ChildSlot
	[
		SAssignNew(VerticalBoxWidget, SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(5.0f, 2.0f)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Enabled"))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(5.0f, 2.0f)
			.AutoWidth()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(FOnCheckStateChanged::CreateSP(this, &SMidPropertySection::OnEnabledBoxChanged))
				.IsChecked(this, &SMidPropertySection::IsEnabledBoxChecked)
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(5.0f, 2.0f)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::FromString(DisplayTextureName))
				.IsEnabled(this, &SMidPropertySection::IsEnabled)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(5.0f, 2.0f)
			.AutoWidth()
			[
				SNew(SObjectPropertyEntryBox)

				.ObjectPath(TAttribute<FString>(this, &SMidPropertySection::GetTexturePath))
				.OnObjectChanged(FOnSetObject::CreateSP(this, &SMidPropertySection::OnEntryTextureChanged))

				.AllowedClass(UTexture::StaticClass())
				.DisplayBrowse(true)
				.EnableContentPicker(true)
				.DisplayThumbnail(true)

				.ThumbnailPool(AssetThumbnailPool)
				.IsEnabled(this, &SMidPropertySection::IsEnabled)
			]
		]
	];
}

void SMidPropertySection::OnEntryTextureChanged(const FAssetData& InData)
{
	TexturePath = InData.GetObjectPathString();
}

FString SMidPropertySection::GetTexturePath() const
{
	return TexturePath;
}

void SMidPropertySection::OnEnabledBoxChanged(ECheckBoxState NewState)
{
	bIsEnabledBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
}

ECheckBoxState SMidPropertySection::IsEnabledBoxChecked() const
{
	return bIsEnabledBoxChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

bool SMidPropertySection::IsEnabled() const
{
	return bIsEnabledBoxChecked;
}

void SMidPropertySection::AddExpressionsToMaterial(UMaterial* Material)
{

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
