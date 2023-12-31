// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUpgradesWidget.h"
#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PDTower.h"
#include "CustomUtils.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPDUpgradesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpgradeCostText->SetText(FText::FromString(FString::FromInt(GetCurrentUpgradeCost()) + " Power"));
	UpgradeButton->OnClicked.AddDynamic(this, &UPDUpgradesWidget::OnUpgradeButtonClicked);
	AssignAnimations();

	EnabledPurchaseButtonStyle = UpgradeButton->WidgetStyle;
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

	UpgradeButton->SetStyle(CanAffordNextUpgrade() ? EnabledPurchaseButtonStyle : DisabledPurchaseButtonStyle);
}

FReply UPDUpgradesWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UPDUpgradesWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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
		//UpgradeButton->SetVisibility(ESlateVisibility::Visible);
		UpgradeButton->SetStyle(EnabledPurchaseButtonStyle);
	}
	else
	{
		UpgradeCostText->SetText(FText::GetEmpty());
		//UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
		UpgradeButton->SetStyle(DisabledPurchaseButtonStyle);
	}
}

void UPDUpgradesWidget::SetUpgradeDescription(FText Description)
{
	UpgradeDescription->SetText(Description);
}

void UPDUpgradesWidget::SetCurrentDescription(FText Description)
{
	CurrentDescription->SetText(Description);
}

void UPDUpgradesWidget::SwipeIn()
{
	if (Animations.Contains("SwipeIn_INST"))
	{
		float AnimTime = 0;
		if (IsPlayingAnimation() && Animations.Contains("SwipeOut_INST"))
		{
			float EndTime = Animations["SwipeOut_INST"]->GetEndTime();
			AnimTime = EndTime - GetAnimationCurrentTime(Animations["SwipeOut_INST"]);
		}

		StopAllAnimations();
		PlayAnimationAtTime(Animations["SwipeIn_INST"], AnimTime);
	}
}

void UPDUpgradesWidget::SwipeOut()
{
	if (Animations.Contains("SwipeOut_INST"))
	{
		float AnimTime = 0;
		if (IsPlayingAnimation() && Animations.Contains("SwipeIn_INST"))
		{
			float EndTime = Animations["SwipeIn_INST"]->GetEndTime();
			AnimTime = EndTime - GetAnimationCurrentTime(Animations["SwipeIn_INST"]);
		}

		StopAllAnimations();
		PlayAnimationAtTime(Animations["SwipeOut_INST"], AnimTime);
	}
}

void UPDUpgradesWidget::UpdateDisplay()
{
	if (ParentTower == nullptr) return;

	SetUpgradeDescription(ParentTower->GetUpgradeDescription());
	SetCurrentDescription(ParentTower->GetCurrentDescription());

	int NewUpgradeCost;

	if (TryGetCurrentUpgradeCost(NewUpgradeCost))
	{
		UpgradeCostText->SetText(FText::FromString(FString::FromInt(NewUpgradeCost) + " Power"));
	}
	else
	{
		UpgradeCostText->SetText(FText::GetEmpty());
		//UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
		UpgradeButton->SetStyle(DisabledPurchaseButtonStyle);
	}

	TowerIcon->SetBrushFromTexture(ParentTower->GetIcon());
}

void UPDUpgradesWidget::OnUpgradeButtonClicked()
{
	if (ParentTower == nullptr) return;

	int UpgradeCost;
	int NewUpgradeCost;
	if (TryGetCurrentUpgradeCost(UpgradeCost) && CanAffordNextUpgrade())
	{
		PlayerWidget->GetPlayer()->UpdatePower(-UpgradeCost);
		CurrentUpgradeIndex++;

		ParentTower->Upgrade();

		if (TryGetCurrentUpgradeCost(NewUpgradeCost))
		{
			UpgradeCostText->SetText(FText::FromString(FString::FromInt(NewUpgradeCost) + " Power"));
		}
		else
		{
			UpgradeCostText->SetText(FText::GetEmpty());
			//UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
			UpgradeButton->SetStyle(DisabledPurchaseButtonStyle);
		}
	}
}

bool UPDUpgradesWidget::CanAffordNextUpgrade() const
{
	if (!UpgradeCosts.IsValidIndex(CurrentUpgradeIndex) || PlayerWidget == nullptr)
	{
		return false;
	}
	return PlayerWidget->GetPlayerPower() >= GetCurrentUpgradeCost();
}
