// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUpgradesWidget.h"
#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PDTower.h"
#include "CustomUtils.h"

void UPDUpgradesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpgradeCostText->SetText(FText::FromString(FString::FromInt(GetCurrentUpgradeCost()) + " Power"));
	UpgradeButton->OnClicked.AddDynamic(this, &UPDUpgradesWidget::OnUpgradeButtonClicked);
}

void UPDUpgradesWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (PlayerWidget == nullptr)
	{
		PlayerWidget = UCustomUtils::GetWorldPlayer(GetWorld(), 0)->GetWidget();
	}

	UpgradeButton->SetIsEnabled(CanAffordNextUpgrade());
}

int UPDUpgradesWidget::GetCurrentUpgradeCost() const
{
	if (!UpgradeCosts.IsValidIndex(CurrentUpgradeIndex))
	{
		return 0;
	}
	return UpgradeCosts[CurrentUpgradeIndex];
}

bool UPDUpgradesWidget::TryGetCurrentUpgradeCost(int& UpgradeCost) const
{
	if (!UpgradeCosts.IsValidIndex(CurrentUpgradeIndex))
	{
		return false;
	}
	UpgradeCost = UpgradeCosts[CurrentUpgradeIndex];
	return true;
}

void UPDUpgradesWidget::SetParentTower(APDTower* NewParentTower)
{
	ParentTower = NewParentTower;
	if (ParentTower != nullptr)
	{
		InitializeUpgradeCosts(ParentTower->GetUpgradeCosts());
	}
}

void UPDUpgradesWidget::InitializeUpgradeCosts(const TArray<int>& TargetCosts)
{
	UpgradeCosts = TArray<int>(TargetCosts);
	int UpgradeCost;
	if (TryGetCurrentUpgradeCost(UpgradeCost))
	{
		UpgradeCostText->SetText(FText::FromString(FString::FromInt(UpgradeCost) + " Power"));
	}
}

void UPDUpgradesWidget::OnUpgradeButtonClicked()
{
	int UpgradeCost;
	int NewUpgradeCost;
	if (TryGetCurrentUpgradeCost(UpgradeCost) && CanAffordNextUpgrade())
	{
		PlayerWidget->UpdatePower(-UpgradeCost);
		CurrentUpgradeIndex++;

		if (TryGetCurrentUpgradeCost(NewUpgradeCost))
		{
			UpgradeCostText->SetText(FText::FromString(FString::FromInt(NewUpgradeCost) + " Power"));
		}
		else
		{
			UpgradeCostText->SetText(FText::GetEmpty());
			UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
		}

		ParentTower->Upgrade();
	}
}

bool UPDUpgradesWidget::CanAffordNextUpgrade() const
{
	if (!UpgradeCosts.IsValidIndex(CurrentUpgradeIndex) || PlayerWidget == nullptr)
	{
		return false;
	}
	return PlayerWidget->GetPower() >= GetCurrentUpgradeCost();
}
