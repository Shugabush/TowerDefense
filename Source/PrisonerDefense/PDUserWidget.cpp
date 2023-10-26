// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PrisonerDefenseGameModeBase.h"

int UPDUserWidget::GetPower() const
{
	return Power;
}

void UPDUserWidget::UpdatePower(const int additionalPower)
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

	TurretPurchasable->OnPurchase.AddDynamic(this, &UPDUserWidget::OnTurretButtonClicked);
	PowerGeneratorPurchasable->OnPurchase.AddDynamic(this, &UPDUserWidget::OnPowerGeneratorButtonClicked);

	PlayButton->OnClicked.AddDynamic(this, &UPDUserWidget::OnPlayButtonClicked);
}

void UPDUserWidget::OnTurretButtonClicked()
{
	OwningPlayer->OnTurretButtonClicked();
}

void UPDUserWidget::OnPowerGeneratorButtonClicked()
{
	OwningPlayer->OnPowerGeneratorButtonClicked();
}

void UPDUserWidget::OnPlayButtonClicked()
{
	OwningPlayer->GetGameMode()->StartRound();
}
