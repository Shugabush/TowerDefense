// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUserWidget.h"
#include "PDPlayer.h"

int UPDUserWidget::GetPower() const
{
	return Power;
}

void UPDUserWidget::UpdatePower(int additionalPower)
{
	Power += additionalPower;
	PowerText->SetText(FText::FromString("Power: " + FString::FromInt(Power)));
}

int UPDUserWidget::GetTurretCost() const
{
	return TurretPurchasable->GetPrice();
}

int UPDUserWidget::GetPowerGeneratorCost() const
{
	return PowerGeneratorPurchasable->GetPrice();
}

void UPDUserWidget::NativeConstruct()
{
	OwningPlayer = GetOwningPlayerPawn<APDPlayer>();
	TurretPurchasable->ParentWidget = this;
	PowerGeneratorPurchasable->ParentWidget = this;

	TurretPurchasable->OnPurchase.AddDynamic(OwningPlayer, &APDPlayer::OnTurretButtonClicked);
	PowerGeneratorPurchasable->OnPurchase.AddDynamic(OwningPlayer, &APDPlayer::OnPowerGeneratorButtonClicked);
}
