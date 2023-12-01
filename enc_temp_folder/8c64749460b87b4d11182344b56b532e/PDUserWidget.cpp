// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUserWidget.h"
#include "PDPlayer.h"
#include "PrisonerDefenseGameModeBase.h"
#include "PDPurchaseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"

int UPDUserWidget::GetPower() const
{
	return Power;
}

void UPDUserWidget::UpdatePower(const int additionalPower)
{
	Power += additionalPower;
	PowerText->SetText(FText::FromString(PowerTextPrefix + FString::FromInt(Power)));
	
	TurretPurchasable->SetButtonEnabled(Power >= TurretPurchasable->GetPrice());
	PowerGeneratorPurchasable->SetButtonEnabled(Power >= PowerGeneratorPurchasable->GetPrice());
}

int UPDUserWidget::GetTurretCost() const
{
	return TurretPurchasable->GetPrice();
}

int UPDUserWidget::GetPowerGeneratorCost() const
{
	return PowerGeneratorPurchasable->GetPrice();
}

void UPDUserWidget::PurchaseTurret()
{
	Power -= GetTurretCost();
	TurretPurchasable->OnPurchased();
}

void UPDUserWidget::PurchasePowerGenerator()
{
	Power -= GetPowerGeneratorCost();
	PowerGeneratorPurchasable->OnPurchased();
}

APDPlayer* UPDUserWidget::GetPlayer() const
{
	return OwningPlayer;
}

void UPDUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
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
	TurretPurchasable->ParentWidget = this;
	PowerGeneratorPurchasable->ParentWidget = this;

	PowerText->SetText(FText::FromString(PowerTextPrefix + FString::FromInt(Power)));
	RoundText->SetText(FText::FromString(RoundTextPrefix + FString::FromInt(GameMode->GetCurrentRoundNumber())));

	TurretPurchasable->OnButtonClicked.AddDynamic(this, &UPDUserWidget::OnTurretButtonClicked);
	PowerGeneratorPurchasable->OnButtonClicked.AddDynamic(this, &UPDUserWidget::OnPowerGeneratorButtonClicked);

	PlayButton->OnClicked.AddDynamic(this, &UPDUserWidget::OnPlayButtonClicked);

	GameMode->OnRoundChanged.AddDynamic(this, &UPDUserWidget::OnRoundChanged);
	GameMode->OnRoundStarted.AddDynamic(this, &UPDUserWidget::OnRoundStarted);
	GameMode->OnRoundEnded.AddDynamic(this, &UPDUserWidget::OnRoundEnded);
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

void UPDUserWidget::OnRoundChanged(int NewRound)
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
