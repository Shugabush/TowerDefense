// Fill out your copyright notice in the Description page of Project Settings.


#include "Round.h"
#include "PrisonerDefenseGameModeBase.h"
#include "PDPrisonerCage.h"
#include "PDPrisoner.h"

#include "Kismet/GameplayStatics.h"

void FGameRound::Initialize(APrisonerDefenseGameModeBase* gameMode)
{
	SpawnTimer = FCooldownTimer(SpawnInterval);
	SpawnTimer.Tick(SpawnInterval);

	GameMode = gameMode;
}

int FGameRound::GetPowerReward() const
{
	return PowerReward;
}

int FGameRound::GetMaxHealth() const
{
	return MaxHealth;
}

void FGameRound::Tick(float DeltaTime)
{
	if (PrisonersSpawned < PrisonersToSpawn)
	{
		if (SpawnTimer.OutOfTime())
		{
			// Spawn a new prisoner
			APDPrisoner* Prisoner = GameMode->GetRandomPrisonerCage()->SpawnPrisoner();
			PrisonersSpawned++;
			SpawnTimer.Reset();
			Prisoners.Add(Prisoner);
		}
		else
		{
			SpawnTimer.Tick(DeltaTime);
		}
	}
	else if (AllPrisonersDefeated())
	{
		// The round is complete
		GameMode->EndRound();
	}
}

bool FGameRound::AllPrisonersDefeated() const
{
	for (auto Prisoner : Prisoners)
	{
		if (Prisoner != nullptr && !Prisoner->IsDefeated())
		{
			return false;
		}
	}
	return true;
}
