// Fill out your copyright notice in the Description page of Project Settings.


#include "SNormalPropertySection.h"

void SNormalPropertySection::Construct(const FArguments& InArgs)
{
	DisplayTextureName = TEXT("Normal Map");

	SMidPropertySection::Construct(InArgs);
}

void SNormalPropertySection::AddExpressionsToMaterial(UMaterial* Material)
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
	UMaterialExpressionTextureSampleParameter2D* TextureExp = UMidterialBPLibrary::AddTextureParameter(Material, Texture, "Normal", FIntPoint(-800, 200));

	TextureCoordinateExp->ConnectExpression(&TextureExp->Coordinates, 0);

	Material->GetEditorOnlyData()->Normal.Connect(0, TextureExp);
}