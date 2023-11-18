// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerDefenseGameModeBase.h"

#include "PDPlayer.h"
#include "PDHUD.h"
#include "PDUserWidget.h"
#include "Round.h"
#include "PDPrisonerCage.h"
#include "PDPlayer.h"
#include "PDGameOverWidget.h"

#include "Kismet/GameplayStatics.h"
#include "CustomUtils.h"

#include "UObject/ConstructorHelpers.h"

APrisonerDefenseGameModeBase::APrisonerDefenseGameModeBase()
{
	DefaultPawnClass = APDPlayer::StaticClass();
	HUDClass = APDHUD::StaticClass();

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

bool APrisonerDefenseGameModeBase::RoundIsRunning() const
{
	return PrisonersShouldSpawn;
}

bool APrisonerDefenseGameModeBase::GameIsRunning() const
{
	return GameRunning;
}

APDPlayer* APrisonerDefenseGameModeBase::GetPlayer() const
{
	return Player;
}

int APrisonerDefenseGameModeBase::GetNewMaxHealth() const
{
	FGameRound Round;
	if (GetCurrentRound(Round))
	{
		return Round.GetMaxHealth();
	}
	return 2;
}

void APrisonerDefenseGameModeBase::StartRound()
{
	FGameRound Round;
	if (GetCurrentRound(Round))
	{
		OnRoundStarted.Broadcast();

		PrisonersShouldSpawn = true;
	}
}

void APrisonerDefenseGameModeBase::EndRound()
{
	OnRoundEnded.Broadcast();

	PrisonersShouldSpawn = false;
	FGameRound Round;
	if (GetCurrentRound(Round))
	{
		Widget->UpdatePower(Round.GetPowerReward());
	}
	RoundIndex++;
	if (GetCurrentRound(Round))
	{
		OnRoundChanged.Broadcast(GetCurrentRoundNumber());
	}
	else
	{
		// No more rounds to complete, so we win.
		UPDGameOverWidget* GameOverWidget;
		Player->GetHUD()->EnableWidget<UPDGameOverWidget>(UPDGameOverWidget::StaticClass(), GameOverWidget, true);
		GameOverWidget->SetResultsText(true);
		GetWorld()->GetWorldSettings()->SetTimeDilation(0);
		Player->GetGameMode()->Victory();
	}
}

int APrisonerDefenseGameModeBase::GetCurrentRoundNumber() const
{
	return RoundIndex + 1;
}

bool APrisonerDefenseGameModeBase::GetCurrentRound(FGameRound& Round) const
{
	if (!Rounds.IsValidIndex(RoundIndex)) { return false; }
	Round = Rounds[RoundIndex];
	return true;
}

APDPrisonerCage* APrisonerDefenseGameModeBase::GetRandomPrisonerCage() const
{
	int RandomIndex = FMath::RandRange(0, PrisonerCages.Num() - 1);
	return PrisonerCages[RandomIndex];
}

void APrisonerDefenseGameModeBase::StartPlay()
{
	Super::StartPlay();

	GameRunning = true;

	// Find prisoner cages
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APDPrisonerCage::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		APDPrisonerCage* Cage = Cast<APDPrisonerCage>(Actor);
		PrisonerCages.Add(Cage);
	}
	
	// Initialize rounds
	for (size_t i = 0; i < Rounds.Num(); i++)
	{
		Rounds[i].Initialize(this);
	}

	Player = UCustomUtils::GetWorldPlayer(GetWorld(), 0);
	HUD = Cast<APDHUD>(HUDClass->GetDefaultObject());
}

void APrisonerDefenseGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Widget == nullptr && Player != nullptr)
	{
		Widget = Player->GetWidget();
	}

	if (RoundIsRunning())
	{
		if (Rounds.IsValidIndex(RoundIndex))
		{
			Rounds[RoundIndex].Tick(DeltaTime);
		}
	}
}

void APrisonerDefenseGameModeBase::Victory()
{
	OnVictory.Broadcast();
}

void APrisonerDefenseGameModeBase::Loss()
{
	OnLoss.Broadcast();
}

