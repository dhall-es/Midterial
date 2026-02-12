// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"

#include "MidterialBPLibrary.h"

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MIDTERIAL_API SMidterialWidgetMI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMidterialWidgetMI)
	{}
	SLATE_END_ARGS()

	// Constructs this widget with InArgs
	void Construct(const FArguments& InArgs);

	// Delegate for when Material Property Entry Box is changed
	void OnEntryMaterialChanged(const FAssetData& InData);

	// Delegate so Material Property Entry Box knows what to display
	FString GetMaterialPath() const;

	// The path of the user-set parent material
	FString MaterialPath;

	// The thumbnail pool for the Material Property Entry Box so thumbnails can be displayed
	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;

	// Delegate for when Build Material button is clicked
	FReply OnBuildButtonClicked();
	
	void AddListItem(TSharedPtr<FString> Item);

	// Adds a new textbox with the string to the list
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);

	// The list of strings
	TArray<TSharedPtr<FString>> Items;

	// The actual UI list
	TSharedPtr<SListView<TSharedPtr<FString>>> ListViewWidget;
};
