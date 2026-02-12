// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"

#include "MidterialBPLibrary.h"
//#include "MaterialEditingLibrary.h" // MaterialEditor
//#include "Materials/MaterialExpressionTextureSampleParameter2D.h" // Engine
//#include "Materials/MaterialExpressionScalarParameter.h" // Engine
//#include "Materials/MaterialExpressionVectorParameter.h" // Engine
//#include "Materials/MaterialExpressionMultiply.h" // Engine
//#include "Materials/MaterialExpressionTextureCoordinate.h" // Engine

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MIDTERIAL_API SMidPropertySection : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMidPropertySection)
	{}
	SLATE_ARGUMENT(FString, TextureName)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void OnEntryTextureChanged(const FAssetData& InData);

	FString GetTexturePath() const;

	void OnEnabledBoxChanged(ECheckBoxState NewState);

	ECheckBoxState IsEnabledBoxChecked() const;

	bool IsEnabled() const;

	void AddExpressionsToMaterial(UMaterial* Material);

protected:
	TSharedPtr<SVerticalBox> VerticalBoxWidget;
	
	FString TextureName;
	FString TexturePath;

	bool bIsEnabledBoxChecked;

	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;
};
