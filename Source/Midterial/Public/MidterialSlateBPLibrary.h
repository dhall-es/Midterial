// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Midterial.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Widgets/SWindow.h"
#include "Editor/EditorWidgets/Public/SAssetDropTarget.h"

#include "SMidterialWidgetMI.h"

#include "MidterialSlateBPLibrary.generated.h"

UCLASS()
class UMidterialSlateBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Midterial|Slate")
	static void LoadMidterialMIWindow(TArray<UObject*> Textures);
};
