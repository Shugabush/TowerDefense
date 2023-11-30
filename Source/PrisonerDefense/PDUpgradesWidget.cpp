// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUpgradesWidget.h"
#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PDTower.h"
#include "CustomUtils.h"
#include "CustomUtils.h"

void UPDUpgradesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpgradeCostText->SetText(FText::FromString(FString::FromInt(GetCurrentUpgradeCost()) + " Power"));
	UpgradeButton->OnClicked.AddDynamic(this, &UPDUpgradesWidget::OnUpgradeButtonClicked);
	AssignAnimations();
}

void UPDUpgradesWidget::AssignAnimations()
{
	UProperty* prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (prop != nullptr)
	{
		if (prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

			// Only want the properties that are widget animations
			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);

				if (widgetAnim != nullptr)
				{
					FString animName;
					widgetAnim->GetName(animName);
					Animations.Add(animName, widgetAnim);
				}
			}
		}

		prop = prop->PropertyLinkNext;
	}
}

void UPDUpgradesWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Player == nullptr)
	{
		Player = UCustomUtils::GetWorldPlayer(GetWorld(), 0);
	}

	if (PlayerWidget == nullptr)
	{
		PlayerWidget = Player->GetWidget();
	}

	if (ParentTower != nullptr)
	{
		CurrentUpgradeIndex = ParentTower->GetCurrentUpgradeIndex();
	}

	UpgradeButton->SetIsEnabled(CanAffordNextUpgrade());
}

FReply UPDUpgradesWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UPDUpgradesWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
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
		UpgradeButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPDUpgradesWidget::SetDescription(FText Description)
{
	UpgradeDescription->SetText(Description);
}

void UPDUpgradesWidget::SwipeIn()
{
	if (Animations.Contains("SwipeIn_INST"))
	{
		PlayAnimation(Animations["SwipeIn_INST"]);
	}
}

void UPDUpgradesWidget::SwipeOut()
{
	if (Animations.Contains("SwipeOut_INST"))
	{
		PlayAnimation(Animations["SwipeOut_INST"]);
	}
}

void UPDUpgradesWidget::UpdateDisplay()
{
	SetDescription(ParentTower->GetUpgradeDescription());

	int NewUpgradeCost;

	if (TryGetCurrentUpgradeCost(NewUpgradeCost))
	{
		UpgradeCostText->SetText(FText::FromString(FString::FromInt(NewUpgradeCost) + " Power"));
	}
	else
	{
		UpgradeCostText->SetText(FText::GetEmpty());
		UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
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

		ParentTower->Upgrade();

		if (TryGetCurrentUpgradeCost(NewUpgradeCost))
		{
			UpgradeCostText->SetText(FText::FromString(FString::FromInt(NewUpgradeCost) + " Power"));
		}
		else
		{
			UpgradeCostText->SetText(FText::GetEmpty());
			UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
		}
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
