// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUpgradesWidget.h"
#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PDTower.h"
#include "CustomUtils.h"

void UPDUpgradesWidget::NativeConstruct()
{
	UpgradeCostText->SetText(FText::FromString("Upgrade for " + FString::FromInt(GetCurrentUpgradeCost()) + " Power"));
	UpgradeButton->OnClicked.AddDynamic(this, &UPDUpgradesWidget::OnUpgradeButtonClicked);

	PlayerWidget = UCustomUtils::GetWorldPlayer(GetWorld(), 0)->GetWidget();
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

void UPDUpgradesWidget::OnUpgradeButtonClicked()
{
	if (CanAffordNextUpgrade())
	{
		PlayerWidget->UpdatePower(-GetCurrentUpgradeCost());
		CurrentUpgradeIndex++;
		UpgradeCostText->SetText(FText::FromString("Upgrade for " + FString::FromInt(GetCurrentUpgradeCost()) + " Power"));

		ParentTower->Upgrade();
	}
}

bool UPDUpgradesWidget::CanAffordNextUpgrade() const
{
	if (!UpgradeCosts.IsValidIndex(CurrentUpgradeIndex))
	{
		return false;
	}
	return PlayerWidget->GetPower() > GetCurrentUpgradeCost();
}
