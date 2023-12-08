// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PrisonerDefenseGameModeBase.h"
#include "PDPurchaseWidget.h"
#include "CustomUtils.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "Kismet/GameplayStatics.h"

float UPDUserWidget::GetPlayerPower() const
{
	return OwningPlayer->GetPower();
}

int UPDUserWidget::GetTurretCost() const
{
	return TurretPurchasable->GetPrice();
}

int UPDUserWidget::GetPowerGeneratorCost() const
{
	return PowerGeneratorPurchasable->GetPrice();
}

APDPlayer* UPDUserWidget::GetPlayer() const
{
	return OwningPlayer;
}

void UPDUserWidget::PurchaseTargetWidget()
{
	if (TargetPurchasable == nullptr) return;

	// Store current price because OnPurchased will change it
	// We need to update power after purchase has been made or else it will check
	// whether the player can afford it using the old price
	float CurrentPrice = TargetPurchasable->GetPrice();
	TargetPurchasable->OnPurchased();
	OwningPlayer->UpdatePower(-CurrentPrice);
}

APrisonerDefenseGameModeBase* UPDUserWidget::GetGameMode() const
{
	return GameMode;
}

void UPDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	OwningPlayer = GetOwningPlayerPawn<APDPlayer>();

	TArray<UWidget*> WidgetChildren = Panel->GetAllChildren();
	for (auto Widget : WidgetChildren)
	{
		UPDPurchaseWidget* PurchaseWidget = Cast<UPDPurchaseWidget>(Widget);
		if (PurchaseWidget != nullptr)
		{
			ChildPurchaseWidgets.Add(PurchaseWidget);
			PurchaseWidget->ParentWidget = this;
		}
	}

	PowerText->SetText(FText::FromString(PowerTextPrefix + UCustomUtils::SanitizeFloat(GetPlayerPower(), 2, 2)));
	RoundText->SetText(FText::FromString(RoundTextPrefix + FString::FromInt(GameMode->GetCurrentRoundNumber())));

	PlayButton->OnClicked.AddDynamic(this, &UPDUserWidget::OnPlayButtonClicked);

	GameMode->OnRoundChanged.AddDynamic(this, &UPDUserWidget::OnRoundChanged);
	GameMode->OnRoundStarted.AddDynamic(this, &UPDUserWidget::OnRoundStarted);
	GameMode->OnRoundEnded.AddDynamic(this, &UPDUserWidget::OnRoundEnded);

	OwningPlayer->OnPowerChanged.AddDynamic(this, &UPDUserWidget::OnPowerUpdated);
}

void UPDUserWidget::OnPowerUpdated(const float PowerValue)
{
	PowerText->SetText(FText::FromString(PowerTextPrefix + UCustomUtils::SanitizeFloat(PowerValue, 2, 2)));

	for (auto Purchasable : ChildPurchaseWidgets)
	{
		Purchasable->OnPowerUpdated(PowerValue);
	}
}

void UPDUserWidget::OnPlayButtonClicked()
{
	OwningPlayer->GetGameMode()->StartRound();
}

void UPDUserWidget::OnRoundChanged(const int NewRound)
{
	RoundText->SetText(FText::FromString(RoundTextPrefix + FString::FromInt(NewRound)));
}

void UPDUserWidget::OnRoundStarted()
{
	PlayButton->SetIsEnabled(false);
}

void UPDUserWidget::OnRoundEnded()
{
	PlayButton->SetIsEnabled(true);
}
