// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Midterial.h"

#include "AssetToolsModule.h" // AssetTools
#include "Materials/Material.h" // Engine
#include "Materials/MaterialInstanceConstant.h" // Engine	

#include "Factories/MaterialFactoryNew.h" // UnrealEd
#include "Factories/MaterialInstanceConstantFactoryNew.h" // UnrealEd

#include "MaterialEditingLibrary.h" // MaterialEditor
#include "Materials/MaterialExpressionTextureSampleParameter2D.h" // Engine
#include "Materials/MaterialExpressionScalarParameter.h" // Engine
#include "Materials/MaterialExpressionVectorParameter.h" // Engine
#include "Materials/MaterialExpressionMultiply.h" // Engine
#include "Materials/MaterialExpressionTextureCoordinate.h" // Engine

#include "MidterialBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

class UFactory;

class UMaterial;

class UMaterialExpressionTextureSampleParameter2D;
class UMaterialExpressionParameter;
class UMaterialExpressionScalarParameter;
class UMaterialExpressionVectorParameter;
class UMaterialExpressionMultiply;
class UMaterialExpressionTextureCoordinate;

UCLASS()
class UMidterialBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Create an asset.
	* 
	* Sourced from https://www.youtube.com/watch?v=oFp3MKu4MoY
	* 
	* @param AssetPath			The path of where to create the asset: "/Game/Folder/MyAsset"
	* @param AssetClass			The class of the asset to create
	* @param AssetFactory		Optional. The factory to use to create the asset
	* @param bOutSuccess		If the action was a success or not
	* @param OutInfoMessage		More information about the action's result
	* 
	* @return The created asset
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial|Assets")
	static UObject* CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory, bool& bOutSuccess,
		FString& OutInfoMessage);

	/**
	* Create a Material asset.
	* 
	* @param AssetPath			The path of where to create the asset: "/Game/Folder/MyAsset"
	* @param bOutSuccess		If the action was a success or not
	* @param OutInfoMessage		More information about the action's result
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial|Assets")
		static UMaterial* CreateMaterialAsset(FString AssetPath, bool& bOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, Category = "Midterial|Assets")
	static UMaterialInstanceConstant* CreateMaterialInstanceAsset(FString AssetPath, UMaterialInterface* InitialParent, bool& bOutSuccess, FString& OutInfoMessage);
	/**
	* Build a material graph with the provided values
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param MaterialPath		The path of the material: "/Game/Folder/MyMaterial"
	* @param TexturePath		The path of the texture to add in the material: "/Game/Folder/MyTexture"
	* @param TexCoord			Texture coordinates for the texture
	* @param Color				Color that will be multiplied by the texture
	* @param Metallic			Metallic value of the material
	* @param Specular			Specular value of the material
	* @param Roughness			Roughness value of the material
	* @param bOutSuccess		If the action was a success or not
	* @param OutInfoMessage		More information about the action's result
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static void BuildMaterialSingleTexture(FString MaterialPath, UTexture* Texture, FVector2D TexCoord, FLinearColor Color,
			float Metallic, float Specular, float Roughness, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	* Retrieve an existing material expression based on the provided name or description
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param Material				The material in which the expression is
	* @param NameOrDescription		The name or description of the expression to find
	* 
	* @return The expression
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static UMaterialExpression* GetExistingMaterialExpressionFromName(UMaterial* Material, FString NameOrDescription);

	/**
	* Add a texture parameter to a material graph
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param Material			The material in which to add the parameter
	* @param Texture			The texture to use
	* @param ParameterName		The name of the expression
	* @param NodePos			The XY coordinates of the node in the graph
	* 
	* @return The expression
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static UMaterialExpressionTextureSampleParameter2D* AddTextureParameter(UMaterial* Material, UTexture* Texture, 
			FString ParameterName, FIntPoint NodePos);
	/**
	* Add a scalar parameter to a material graph
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param Material			The material in which to add the parameter
	* @param Value				The float value to use
	* @param ParameterName		The name of the expression
	* @param NodePos			The XY coordinates of the node in the graph
	* 
	* @return The expression
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static UMaterialExpressionScalarParameter* AddScalarParameter(UMaterial* Material, float Value, FString ParameterName,
			FIntPoint NodePos);
	/**
	* Add a vector parameter to a material graph
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param Material			The material in which to add the parameter
	* @param Color				The color value to use
	* @param ParameterName		The name of the expression
	* @param NodePos			The XY coordinates of the node in the graph
	* 
	* @return The expression
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static UMaterialExpressionVectorParameter* AddVectorParameter(UMaterial* Material, FLinearColor Color,
			FString ParameterName, FIntPoint NodePos);

	/**
	* Add a multiply expression to a material graph
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param Material			The material in which to add the expression
	* @param ExpressionDesc		The description of the expression
	* @param NodePos			The XY coordinates of the node in the graph
	* 
	* @return The expression
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static UMaterialExpressionMultiply* AddMultiplyExpression(UMaterial* Material, FString ExpressionDesc,
			FIntPoint NodePos);
	
	/**
	* Add a texture coordinate expression to a material graph
	* 
	* Sourced from https://www.youtube.com/watch?v=Fd56hSN83mk
	* 
	* @param Material			The material in which to add the expression
	* @param Value				The texture coordinates value to use
	* @param ExpressionDesc		The description of the expression
	* @param NodePos			The XY coordinates of the node in the graph
	* 
	* @return The expression
	*/
	UFUNCTION(BlueprintCallable, Category = "Midterial")
		static UMaterialExpressionTextureCoordinate* AddTexCoordExpression(UMaterial* Material, FVector2D Value, FString ExpressionDesc, FIntPoint NodePos);

	UFUNCTION(BlueprintCallable, Category = "Midterial/Testing")
		static void BuildMaterialInstance(FString MaterialInstancePath, UMaterial* InitialParent, TArray<UObject*> Textures,
			bool& bOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, Category = "Midterial/Instance Creation")
		static void BuildMaterialInstanceMatchExtension(UMaterial* InitialParent, TArray<FString> TexturePaths);
};