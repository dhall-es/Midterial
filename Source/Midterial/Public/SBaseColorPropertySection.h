// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMidPropertySection.h"

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MIDTERIAL_API SBaseColorPropertySection : public SMidPropertySection
{
public:
	virtual void Construct(const FArguments& InArgs) override;

	void OnTintBoxChanged(ECheckBoxState NewState);

	ECheckBoxState IsTintBoxChecked() const;

	virtual void AddExpressionsToMaterial(UMaterial* Material) override;

protected:
	bool bIsTintBoxChecked;
};
