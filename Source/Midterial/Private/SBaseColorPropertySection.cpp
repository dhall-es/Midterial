// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseColorPropertySection.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBaseColorPropertySection::Construct(const FArguments& InArgs)
{
	DisplayTextureName = TEXT("Base Color Texture");

	SMidPropertySection::Construct(InArgs);

	VerticalBoxWidget->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.Padding(5.0f, 2.0f)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Enable Tint"))
			.IsEnabled(this, &SMidPropertySection::IsEnabled)
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.Padding(5.0f, 2.0f)
		.AutoWidth()
		[
			SNew(SCheckBox)
			.OnCheckStateChanged(FOnCheckStateChanged::CreateSP(this, &SBaseColorPropertySection::OnTintBoxChanged))
			.IsChecked(this, &SBaseColorPropertySection::IsTintBoxChecked)
			.IsEnabled(this, &SMidPropertySection::IsEnabled)
		]
	];
}

void SBaseColorPropertySection::OnTintBoxChanged(ECheckBoxState NewState)
{
	bIsTintBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
}

ECheckBoxState SBaseColorPropertySection::IsTintBoxChecked() const
{
	return bIsTintBoxChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SBaseColorPropertySection::AddExpressionsToMaterial(UMaterial* Material)
{
	if (SMidPropertySection::IsEnabled() == false)
	{
		return;
	}

	UTexture* Texture = Cast<UTexture>(StaticLoadObject(UObject::StaticClass(), nullptr, *TexturePath));
	if (Texture == nullptr)
	{
		return;
	}

	UMaterialExpressionTextureCoordinate* TextureCoordinateExp = UMidterialBPLibrary::AddTexCoordExpression(Material, FVector2D(0.0f, 0.0f), FIntPoint(-1000, -50));
	UMaterialExpressionTextureSampleParameter2D* TextureExp = UMidterialBPLibrary::AddTextureParameter(Material, Texture, "BaseColor", FIntPoint(-800, -50));

	TextureCoordinateExp->ConnectExpression(&TextureExp->Coordinates, 0);
	
	Material->GetEditorOnlyData()->BaseColor.Connect(0, TextureExp);
}