// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPurchaseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "PrisonerDefenseGameModeBase.h"
#include "CustomUtils.h"

#include "PDUserWidget.h"

void UPDPurchaseWidget::NativeConstruct()
{
	PurchaseButton->WidgetStyle.Normal.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Hovered.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Pressed.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Disabled.SetResourceObject(ButtonIcon);

	PurchaseButton->OnClicked.AddDynamic(this, &UPDPurchaseWidget::TryPurchase);

	PriceText->SetText(FText::FromString(UCustomUtils::SanitizeFloat(Price, 2) + " Power"));
}

void UPDPurchaseWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	PurchaseButton->WidgetStyle.Normal.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Hovered.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Pressed.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Disabled.SetResourceObject(ButtonIcon);
	PriceText->SetText(FText::FromString(UCustomUtils::SanitizeFloat(Price, 2) + " Power"));
}

int UPDPurchaseWidget::GetPrice() const
{
	return Price;
}

void UPDPurchaseWidget::SetButtonEnabled(bool Enabled)
{
	PurchaseButton->SetIsEnabled(Enabled);
	PriceText->SetIsEnabled(Enabled);
}

void UPDPurchaseWidget::OnPurchased()
{
	Price += PriceIncreasePerPurchase;
	PriceText->SetText(FText::FromString(UCustomUtils::SanitizeFloat(Price, 2) + " Power"));
	OnPurchase.Broadcast();
}

void UPDPurchaseWidget::TryPurchase()
{
	if (ParentWidget != nullptr && ParentWidget->GetPlayerPower() >= Price)
	{
		OnButtonClicked.Broadcast();
	}
}
