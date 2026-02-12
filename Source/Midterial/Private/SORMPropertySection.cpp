// Fill out your copyright notice in the Description page of Project Settings.


#include "SORMPropertySection.h"

void SORMPropertySection::Construct(const FArguments& InArgs)
{
	DisplayTextureName = TEXT("ORM Map");

	SMidPropertySection::Construct(InArgs);
}

void SORMPropertySection::AddExpressionsToMaterial(UMaterial* Material)
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
	UMaterialExpressionTextureSampleParameter2D* TextureExp = UMidterialBPLibrary::AddTextureParameter(Material, Texture, "ORM", FIntPoint(-800, 400));

	TextureCoordinateExp->ConnectExpression(&TextureExp->Coordinates, 0);

	Material->GetEditorOnlyData()->AmbientOcclusion.Connect(1, TextureExp);
	Material->GetEditorOnlyData()->Roughness.Connect(2, TextureExp);
	Material->GetEditorOnlyData()->Metallic.Connect(3, TextureExp);
}