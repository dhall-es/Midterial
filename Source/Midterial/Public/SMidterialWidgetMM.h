// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"

#include "MidterialBPLibrary.h"

#include "SMidPropertySection.h"
#include "SBaseColorPropertySection.h"
#include "SNormalPropertySection.h"
#include "SORMPropertySection.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MIDTERIAL_API SMidterialWidgetMM : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMidterialWidgetMM)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// Delegate for when Build Material button is clicked
	FReply OnBuildButtonClicked() const;

	TSharedPtr<SBaseColorPropertySection> BaseColorSection;
	TSharedPtr<SNormalPropertySection> NormalSection;
	TSharedPtr<SORMPropertySection> ORMSection;
};
