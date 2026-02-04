// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Widgets/SWindow.h"

#include "MidterialSlateBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UMidterialSlateBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Midterial|Slate")
	static void ShowMidterialSlate();
};
