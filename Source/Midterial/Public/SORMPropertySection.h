// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMidPropertySection.h"

#include "Widgets/SCompoundWidget.h"

class MIDTERIAL_API SORMPropertySection : public SMidPropertySection
{
public:
	virtual void Construct(const FArguments& InArgs) override;

	virtual void AddExpressionsToMaterial(UMaterial* Material) override;
};
