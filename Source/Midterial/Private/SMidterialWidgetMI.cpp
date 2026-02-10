// Fill out your copyright notice in the Description page of Project Settings.


#include "SMidterialWidgetMI.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMidterialWidgetMI::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.FillHeight(1.0f)
			[
				SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(10.0f, 10.0f)
					[
						SNew(SObjectPropertyEntryBox)
							.ObjectPath(TAttribute<FString>(this, &SMidterialWidgetMI::GetObjectPath))
							.OnObjectChanged(FOnSetObject::CreateSP(this, &SMidterialWidgetMI::OnEntryObjectChanged))
							.OnShouldSetAsset(FOnShouldSetAsset::CreateSP(this, &SMidterialWidgetMI::OnShouldSetEntryAsset))
							.AllowedClass(UMaterial::StaticClass())
							.DisplayThumbnail(true)
							.DisplayBrowse(true)
							.EnableContentPicker(true)
					]
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			.FillHeight(1.0f)
			[
				SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(10.0f, 10.0f)
					[
						SNew(SButton)
							.Text(FText::FromString("Assign Textures"))
							.HAlign(HAlign_Center)
					]
			]
	];
	
	// return void, param echeckboxstate
}

void SMidterialWidgetMI::OnEntryObjectChanged(const FAssetData& InData)
{
	UE_LOG(LogTemp, Warning, TEXT("Entry Object Changed"))
	ObjectPath = InData.GetObjectPathString();
}

bool SMidterialWidgetMI::OnShouldSetEntryAsset(const FAssetData& InData)
{
	UE_LOG(LogTemp, Warning, TEXT("Should Set Entry Asset"))
	return true;
}

FString SMidterialWidgetMI::GetObjectPath() const
{
	return ObjectPath;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
