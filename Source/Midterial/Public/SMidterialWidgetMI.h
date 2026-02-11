// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"

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

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void OnEntryMaterialChanged(const FAssetData& InData);

	FString GetMaterialPath() const;

	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;

	void AddListItem(TSharedPtr<FString> Item);

	/* Adds a new textbox with the string to the list */
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);

	FString MaterialPath;

	/* The list of strings */
	TArray<TSharedPtr<FString>> Items;

	/* The actual UI list */
	TSharedPtr<SListView<TSharedPtr<FString>>> ListViewWidget;
};
