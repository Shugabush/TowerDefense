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
	return TurretCost;
}

bool UPDUserWidget::CanAffordTurret() const
{
	return Power >= TurretCost;
}

void UPDUserWidget::NativeConstruct()
{
	OwningPlayer = GetOwningPlayerPawn<APDPlayer>();
	TurretPurchasable->ParentWidget = this;

	TurretPurchasable->OnPurchase.AddDynamic(OwningPlayer, &APDPlayer::OnTurretButtonClicked);

	/*if (TurretButton != nullptr)
	{
		TurretButton->OnClicked.AddDynamic(OwningPlayer, &APDPlayer::OnTurretButtonClicked);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Turret button hasn't been assigned!"));
	}*/
}
