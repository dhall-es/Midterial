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

	bOutSuccess = true;
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

UMaterialExpression* UMidterialBPLibrary::GetExistingMaterialExpressionFromClass(UMaterial* Material, UClass* Class)
{
	// Loop through expressions in material
	for (UMaterialExpression* Expression : Material->GetExpressions())
	{
		if (Expression->IsA(Class))
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

UMaterialExpressionTextureCoordinate* UMidterialBPLibrary::AddTexCoordExpressionDesc(UMaterial* Material, FVector2D Value,
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

UMaterialExpressionTextureCoordinate* UMidterialBPLibrary::AddTexCoordExpression(UMaterial* Material, FVector2D Value, FIntPoint NodePos)
{
	// Get existing parameter from material
	UMaterialExpressionTextureCoordinate* TextureCoordinateExpression =
		Cast<UMaterialExpressionTextureCoordinate>(GetExistingMaterialExpressionFromClass(Material, UMaterialExpressionTextureCoordinate::StaticClass()));

	// Create new parameter if it doesn't exist
	if (TextureCoordinateExpression == nullptr)
	{
		UMaterialExpression* Expression =
			UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionTextureCoordinate::StaticClass());
		TextureCoordinateExpression = Cast<UMaterialExpressionTextureCoordinate>(Expression);
		Material->AddExpressionParameter(TextureCoordinateExpression, Material->EditorParameters);
	}

	// Set name and position
	TextureCoordinateExpression->MaterialExpressionEditorX = NodePos.X;
	TextureCoordinateExpression->MaterialExpressionEditorY = NodePos.Y;

	// Return parameter
	return TextureCoordinateExpression;
}

void UMidterialBPLibrary::BuildMaterialInstanceMatchExtension(UMaterial* InitialParent, TArray<FString> TexturePaths)
{
	FString TexDirectory{}, TexName{};
	
	// Split first texture path into the directory and filename
	// (e.g. '/Midterial/DefaultTextures/' and 'Substance_graph_ORM.Substance_graph_ORM')
	TexturePaths[0].Split(TEXT("/"), &TexDirectory, &TexName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	FString MaterialName{}, TargetPath{};

	// Remove extension (e.g. '.Substance_graph_ORM')
	if (TexName.Split(TEXT("."), &TexName, nullptr))
	{
		MaterialName = TexName;
	}
	
	// Remove suffix (e.g. '_ORM')
	if (TexName.Split(TEXT("_"), &TexName, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
	{
		MaterialName = TexName;
	}

	TargetPath = TexDirectory + TEXT("/MI_") + MaterialName;

	bool bOutSuccess{};
	FString OutInfoMessage{};

	UMaterialInstanceConstant* MaterialInst = CreateMaterialInstanceAsset(TargetPath, InitialParent, bOutSuccess, OutInfoMessage);

	if (bOutSuccess == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Instance creation failed with message '%s'"), *OutInfoMessage)
		return;
	}

	// Get Texture parameters
	TMap<FMaterialParameterInfo, FMaterialParameterMetadata> TextureParamMap{};
	MaterialInst->GetAllParametersOfType(EMaterialParameterType::Texture, TextureParamMap);

	for (auto& ParamPair : TextureParamMap)
	{
		// Loop through each parameter name
		FString ParamName = ParamPair.Key.Name.ToString();
		UTexture* MatchingTexture{};

		// Loop through each texture path to look for a match
		for (auto& texPath : TexturePaths)
		{
			UObject* oTex = StaticLoadObject(UObject::StaticClass(), nullptr, *texPath);

			// Skip if loaded asset from path is somehow not a texture
			if (oTex->IsA(UTexture::StaticClass()) == false)
			{
				continue;
			}

			// Get extension for texture asset (e.g. 'ORM')
			FString TexExtension{};
			if (oTex->GetName().Split(FString(TEXT("_")), nullptr, &TexExtension, ESearchCase::CaseSensitive, ESearchDir::FromEnd) == false)
			{
				TexExtension = oTex->GetName();
			}

			UE_LOG(LogTemp, Warning, TEXT("Texture path '%s' shortened to extension '%s'"), *texPath, *TexExtension)

			// If the extension matches the name of the current material param,
			// Store the texture and break
			if (TexExtension.Equals(ParamName, ESearchCase::IgnoreCase))
			{
				MatchingTexture = Cast<UTexture>(oTex);
				break;
			}
		}

		// Assign the texture to the param if the texture is set
		if (MatchingTexture != nullptr)
		{
			MaterialInst->SetTextureParameterValueEditorOnly(ParamPair.Key, MatchingTexture);
		}
	}
}