// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPurchaseWidget.h"

#include "PDUserWidget.h"

void UPDPurchaseWidget::NativeConstruct()
{
	PurchaseButton->WidgetStyle.Normal.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Hovered.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Pressed.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Disabled.SetResourceObject(ButtonIcon);

	PurchaseButton->OnClicked.AddDynamic(this, &UPDPurchaseWidget::TryPurchase);

	PriceText->SetText(FText::FromString(FString::FromInt(Price) + " Power"));
}

void UPDPurchaseWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	PurchaseButton->WidgetStyle.Normal.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Hovered.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Pressed.SetResourceObject(ButtonIcon);
	PurchaseButton->WidgetStyle.Disabled.SetResourceObject(ButtonIcon);
	PriceText->SetText(FText::FromString(FString::FromInt(Price) + " Power"));
}

void UPDPurchaseWidget::TryPurchase()
{
	if (ParentWidget != nullptr && ParentWidget->GetPower() >= Price)
	{
		OnPurchase.Broadcast();
	}
}
