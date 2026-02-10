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

	void OnEntryObjectChanged(const FAssetData& InData);

	bool OnShouldSetEntryAsset(const FAssetData& InData);

	FString GetObjectPath() const;
	FString ObjectPath;
};
