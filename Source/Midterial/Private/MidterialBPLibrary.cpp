// Copyright Epic Games, Inc. All Rights Reserved.

#include "MidterialBPLibrary.h"

UMidterialBPLibrary::UMidterialBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

UObject* UMidterialBPLibrary::CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Get the asset tools module
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	// Find the right factory
	UFactory* Factory = AssetFactory;
	if (Factory == nullptr)
	{
		for (UFactory* Fac : AssetTools.GetNewAssetFactories())
		{
			if (Fac->SupportedClass == AssetClass)
			{
				Factory = Fac;
				break;
			}
		}

		// If loop fails to find factory
		if (Factory == nullptr)
		{
			bOutSuccess = false;
			OutInfoMessage = FString::Printf(TEXT("Create Asset Failed - Was not able to find factory for AssetClass. '%s'"), *AssetPath);
			return nullptr;
		}
	}
	// If factory is pre-set, does it support the desired asset?
	else if (Factory->SupportedClass != AssetClass)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Create Asset Failed - Factory cannot create desired AssetClass. '%s'"), *AssetPath);
		return nullptr;
	}

	// Create asset
	UObject* Asset = AssetTools.CreateAsset(FPaths::GetBaseFilename(AssetPath), FPaths::GetPath(AssetPath), AssetClass, Factory);

	// Make sure it actually worked
	if (Asset == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Create Asset Failed - Either the path is invalid or the asset already exists. '%s'"), *AssetPath);
		return nullptr;
	}

	return Asset;
}

UMaterial* UMidterialBPLibrary::CreateMaterialAsset(FString AssetPath, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Factory
	UMaterialFactoryNew* Factory = NewObject<UMaterialFactoryNew>();
	Factory->InitialTexture = nullptr;

	UObject* Asset = CreateAsset(AssetPath, UMaterial::StaticClass(), Factory, bOutSuccess, OutInfoMessage);
	return Cast<UMaterial>(Asset);
}

UMaterialInstanceConstant* UMidterialBPLibrary::CreateMaterialInstanceAsset(FString AssetPath, UMaterialInterface* InitialParent, bool& bOutSuccess, FString& OutInfoMessage)
{
	UMaterialInstanceConstantFactoryNew* Factory = NewObject< UMaterialInstanceConstantFactoryNew>();
	Factory->InitialParent = InitialParent;

	UObject* Asset = CreateAsset(AssetPath, UMaterialInstanceConstant::StaticClass(), Factory, bOutSuccess, OutInfoMessage);
	return Cast<UMaterialInstanceConstant>(Asset);
}

void UMidterialBPLibrary::BuildMaterialSingleTexture(FString MaterialPath, UTexture* Texture, FVector2D TexCoord, FLinearColor Color,
	float Metallic, float Specular, float Roughness, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Get material and texture
	UMaterial* Material = Cast<UMaterial>(StaticLoadObject(UObject::StaticClass(), nullptr, *MaterialPath));

	// Create material if it doesn't exist
	if (Material == nullptr)
	{
		Material = CreateMaterialAsset(MaterialPath, bOutSuccess, OutInfoMessage);

		// If material creation fails, just return
		if (Material == nullptr)
		{
			return;
		}
	}

	// Create nodes
	UMaterialExpressionTextureCoordinate* TextureCoordinateExp = AddTexCoordExpression(Material, TexCoord, "MyTextureCoordinates", FIntPoint(-1000, -50));
	UMaterialExpressionTextureSampleParameter2D* TextureExp = AddTextureParameter(Material, Texture, "MyTexture", FIntPoint(-800, -50));
	UMaterialExpressionVectorParameter* ColorExp = AddVectorParameter(Material, Color, "MyColor", FIntPoint(-800, 200));
	UMaterialExpressionMultiply* ColorMultiplyExp = AddMultiplyExpression(Material, "MyColorMultiply", FIntPoint(-500, 0));
	UMaterialExpressionScalarParameter* MetallicScalarExp = AddScalarParameter(Material, Metallic, "MyMetallic", FIntPoint(-300, 50));
	UMaterialExpressionScalarParameter* SpecularScalarExp = AddScalarParameter(Material, Specular, "MySpecular", FIntPoint(-300, 150));
	UMaterialExpressionScalarParameter* RoughnessScalarExp = AddScalarParameter(Material, Roughness, "MyRoughness", FIntPoint(-300, 250));

	// Connect nodes
	TextureCoordinateExp->ConnectExpression(&TextureExp->Coordinates, 0);
	TextureExp->ConnectExpression(&ColorMultiplyExp->A, 0);
	ColorExp->ConnectExpression(&ColorMultiplyExp->B, 0);

	// Connect to main material node
	Material->GetEditorOnlyData()->BaseColor.Connect(0, ColorMultiplyExp);
	Material->GetEditorOnlyData()->Metallic.Connect(0, MetallicScalarExp);
	Material->GetEditorOnlyData()->Specular.Connect(0, SpecularScalarExp);
	Material->GetEditorOnlyData()->Roughness.Connect(0, RoughnessScalarExp);

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Build Material Succeeded - '%s'"), *MaterialPath);
}

UMaterialExpression* UMidterialBPLibrary::GetExistingMaterialExpressionFromName(UMaterial* Material, FString NameOrDescription)
{
	// Loop through expressions in material
	for (UMaterialExpression* Expression : Material->GetExpressions())
	{
		// If the expression is a parameter, check the description (since the name can't be changed)
		UMaterialExpressionParameter* Parameter = Cast<UMaterialExpressionParameter>(Expression);
		if (Parameter != nullptr && Parameter->ParameterName.ToString() == NameOrDescription)
		{
			return Parameter;
		}
		// Otherwise check the name on nodes that can be renamed
		else if (Expression->Desc == NameOrDescription)
		{
			return Expression;
		}
	}
	return nullptr;
}

/**
* I know that there should be a faster way to do this, involving something like <T>, but idk how so I will leave it to copy+paste for now
*/

UMaterialExpressionTextureSampleParameter2D* UMidterialBPLibrary::AddTextureParameter(UMaterial* Material, UTexture* Texture, 
	FString ParameterName, FIntPoint NodePos)
{
	// Get existing parameter from material
	UMaterialExpressionTextureSampleParameter2D* TextureParameter = 
		Cast<UMaterialExpressionTextureSampleParameter2D>(GetExistingMaterialExpressionFromName(Material, ParameterName));

	// Create new parameter if one doesn't exist
	if (TextureParameter == nullptr)
	{
		UMaterialExpression* Expression = 
			UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionTextureSampleParameter2D::StaticClass());
		TextureParameter = Cast<UMaterialExpressionTextureSampleParameter2D>(Expression);
		Material->AddExpressionParameter(TextureParameter, Material->EditorParameters);
	}

	// Set name and position
	TextureParameter->ParameterName = *ParameterName;
	TextureParameter->MaterialExpressionEditorX = NodePos.X;
	TextureParameter->MaterialExpressionEditorY = NodePos.Y;

	// Set value
	TextureParameter->Texture = Texture;
	

	switch (Texture->CompressionSettings)
	{
	case TC_Normalmap:
		TextureParameter->SamplerType = SAMPLERTYPE_Normal;
		break;

	case TC_Masks:
		TextureParameter->SamplerType = SAMPLERTYPE_Masks;
		break;

	case TC_Grayscale:
		if (Texture->SRGB)
		{
			TextureParameter->SamplerType = SAMPLERTYPE_Grayscale;
			break;
		}
		TextureParameter->SamplerType = SAMPLERTYPE_LinearGrayscale;
		break;

	case TC_Alpha:
		TextureParameter->SamplerType = SAMPLERTYPE_Alpha;
		break;

	case TC_DistanceFieldFont:
		TextureParameter->SamplerType = SAMPLERTYPE_DistanceFieldFont;
		break;

	default:
		if (Texture->SRGB)
		{
			TextureParameter->SamplerType = SAMPLERTYPE_Color;
			break;
		}
		TextureParameter->SamplerType = SAMPLERTYPE_LinearColor;
		break;
	}

	// Return parameter
	return TextureParameter;
}

UMaterialExpressionScalarParameter* UMidterialBPLibrary::AddScalarParameter(UMaterial* Material, float Value, FString ParameterName,
	FIntPoint NodePos)
{
	// Get existing parameter from material
	UMaterialExpressionScalarParameter* ScalarParameter =
		Cast<UMaterialExpressionScalarParameter>(GetExistingMaterialExpressionFromName(Material, ParameterName));

	// Create new parameter if it doesn't exist
	if (ScalarParameter == nullptr)
	{
		UMaterialExpression* Expression = 
			UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionScalarParameter::StaticClass());
		ScalarParameter = Cast<UMaterialExpressionScalarParameter>(Expression);
		Material->AddExpressionParameter(ScalarParameter, Material->EditorParameters);
	}

	// Set name and position
	ScalarParameter->ParameterName = *ParameterName;
	ScalarParameter->MaterialExpressionEditorX = NodePos.X;
	ScalarParameter->MaterialExpressionEditorY = NodePos.Y;

	// Set value
	ScalarParameter->DefaultValue = Value;

	// Return parameter
	return ScalarParameter;
}

UMaterialExpressionVectorParameter* UMidterialBPLibrary::AddVectorParameter(UMaterial* Material, FLinearColor Color,
	FString ParameterName, FIntPoint NodePos)
{
	// Get existing parameter from material
	UMaterialExpressionVectorParameter* VectorParameter =
		Cast<UMaterialExpressionVectorParameter>(GetExistingMaterialExpressionFromName(Material, ParameterName));

	// Create new parameter if it doesn't exist
	if (VectorParameter == nullptr)
	{
		UMaterialExpression* Expression = 
			UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionVectorParameter::StaticClass());
		VectorParameter = Cast<UMaterialExpressionVectorParameter>(Expression);
		Material->AddExpressionParameter(VectorParameter, Material->EditorParameters);
	}

	// Set name and position
	VectorParameter->ParameterName = *ParameterName;
	VectorParameter->MaterialExpressionEditorX = NodePos.X;
	VectorParameter->MaterialExpressionEditorY = NodePos.Y;

	// Set value
	VectorParameter->DefaultValue = Color;

	// Return parameter
	return VectorParameter;
}

UMaterialExpressionMultiply* UMidterialBPLibrary::AddMultiplyExpression(UMaterial* Material, FString ExpressionDesc, FIntPoint NodePos)
{
	// Get existing parameter from material
	UMaterialExpressionMultiply* MultiplyExpression =
		Cast<UMaterialExpressionMultiply>(GetExistingMaterialExpressionFromName(Material, ExpressionDesc));

	// Create new parameter if it doesn't exist
	if (MultiplyExpression == nullptr)
	{
		UMaterialExpression* Expression =
			UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionMultiply::StaticClass());
		MultiplyExpression = Cast<UMaterialExpressionMultiply>(Expression);
		Material->AddExpressionParameter(MultiplyExpression, Material->EditorParameters);
	}

	// Set name and position
	MultiplyExpression->Desc = *ExpressionDesc;
	MultiplyExpression->MaterialExpressionEditorX = NodePos.X;
	MultiplyExpression->MaterialExpressionEditorY = NodePos.Y;

	// Return parameter
	return MultiplyExpression;
}

UMaterialExpressionTextureCoordinate* UMidterialBPLibrary::AddTexCoordExpression(UMaterial* Material, FVector2D Value,
	FString ExpressionDesc, FIntPoint NodePos)
{
	// Get existing parameter from material
	UMaterialExpressionTextureCoordinate* TextureCoordinateExpression =
		Cast<UMaterialExpressionTextureCoordinate>(GetExistingMaterialExpressionFromName(Material, ExpressionDesc));

	// Create new parameter if it doesn't exist
	if (TextureCoordinateExpression == nullptr)
	{
		UMaterialExpression* Expression =
			UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionTextureCoordinate::StaticClass());
		TextureCoordinateExpression = Cast<UMaterialExpressionTextureCoordinate>(Expression);
		Material->AddExpressionParameter(TextureCoordinateExpression, Material->EditorParameters);
	}

	// Set name and position
	TextureCoordinateExpression->Desc = *ExpressionDesc;
	TextureCoordinateExpression->MaterialExpressionEditorX = NodePos.X;
	TextureCoordinateExpression->MaterialExpressionEditorY = NodePos.Y;

	// Return parameter
	return TextureCoordinateExpression;
}

void UMidterialBPLibrary::BuildMaterialInstance(FString MaterialInstancePath, UMaterial* InitialParent, TArray<UObject*> Textures, bool& bOutSuccess, FString& OutInfoMessage)
{
	UMaterialInstanceConstant* MaterialInst = CreateMaterialInstanceAsset(MaterialInstancePath, InitialParent, bOutSuccess, OutInfoMessage);

	/*FString PathName{}, Left{}, Right{};
	PathName = InitialParent->GetPathName(nullptr);

	if (PathName.Split(TEXT("/"), &Left, &Right, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
	{
		OutInfoMessage = Right;
	}*/
	TMap<FMaterialParameterInfo, FMaterialParameterMetadata> TextureParamMap {};
	MaterialInst->GetAllParametersOfType(EMaterialParameterType::Texture, TextureParamMap);

	for (auto& ParamPair : TextureParamMap)
	{
		FString ParamName = ParamPair.Key.Name.ToString();
		UTexture* MatchingTexture {};

		for (auto& oTex : Textures)
		{
			if (oTex->IsA(UTexture::StaticClass()) == false)
			{
				continue;
			}

			FString TexName{};
			oTex->GetName().Split(FString(TEXT("_")), nullptr, &TexName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);

			if (TexName.Equals(ParamName, ESearchCase::IgnoreCase))
			{
				MatchingTexture = Cast<UTexture>(oTex);
			}
		}

		if (MatchingTexture != nullptr)
		{
			MaterialInst->SetTextureParameterValueEditorOnly(ParamPair.Key, MatchingTexture);
		}
	}
}

void UMidterialBPLibrary::BuildMaterialMultiTexture(FString MaterialPath, TArray<UObject*> Textures, bool& bOutSuccess, FString& OutInfoMessage)
{
	//// Get material and texture
	//UMaterial* Material = Cast<UMaterial>(StaticLoadObject(UObject::StaticClass(), nullptr, *MaterialPath));

	//// Create material if it doesn't exist
	//if (Material == nullptr)
	//{
	//	Material = CreateMaterialAsset(MaterialPath, bOutSuccess, OutInfoMessage);

	//	// If material creation fails, just return
	//	if (Material == nullptr)
	//	{
	//		return;
	//	}

	//	
	//}
	
	UTexture* BaseColorTex {};
	UTexture* NormalTex {};
	UTexture* ORMTex {};

	UTexture* AmbientOcclusionTex {};
	UTexture* RoughnessTex {};
	UTexture* MetallicTex {};

	for (auto& oTex : Textures)
	{
		if (oTex->IsA(UTexture::StaticClass()) == false)
		{
			continue;
		}
		FString Name = oTex->GetName();
		UTexture* tTex = Cast<UTexture>(oTex);

		if (Name.Contains(TEXT("basecolor"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			BaseColorTex = tTex;
		}
		else if (Name.Contains(TEXT("normal"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			NormalTex = tTex;
		}
		else if (Name.Contains(TEXT("ambientocclusion"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			AmbientOcclusionTex = tTex;
		}
		else if (Name.Contains(TEXT("roughness"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			RoughnessTex = tTex;
		}
		else if (Name.Contains(TEXT("metallic"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			MetallicTex = tTex;
		}
		// Check ORM last since "normal" contains "orm"
		else if (Name.Contains(TEXT("orm"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			// Double-check the texture isn't a normal map, skip if it is
			if (tTex->CompressionSettings == TC_Normalmap)
			{
				continue;
			}

			ORMTex = tTex;
		}
	}
}

