// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUpgradesWidget.h"

void UPDUpgradesWidget::NativeConstruct()
{
	UpgradeCostText->SetText(FText::FromString("Upgrade for " + FString::FromInt(GetCurrentUpgradeCost()) + " Power"));
}

int UPDUpgradesWidget::GetCurrentUpgradeCost() const
{
	if (!UpgradeCosts.IsValidIndex(CurrentUpgradeIndex))
	{
		return 0;
	}
	return UpgradeCosts[CurrentUpgradeIndex];
}

void UPDUpgradesWidget::InitializeUpgradeCosts(const TArray<int> TargetCosts)
{
	UpgradeCosts = TArray<int>(TargetCosts);
}
