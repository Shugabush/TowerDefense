// Fill out your copyright notice in the Description page of Project Settings.


#include "Round.h"
#include "PrisonerDefenseGameModeBase.h"
#include "PDPrisonerCage.h"

#include "Kismet/GameplayStatics.h"

void FGameRound::Initialize(APrisonerDefenseGameModeBase* gameMode)
{
	SpawnTimer = FCooldownTimer(SpawnInterval);
	SpawnTimer.Tick(SpawnInterval);

	GameMode = gameMode;
}

void FGameRound::Tick(float DeltaTime)
{
	if (PrisonersSpawned < PrisonersToSpawn)
	{
		if (SpawnTimer.OutOfTime())
		{
			// Spawn a new prisoner
			GameMode->GetRandomPrisonerCage()->SpawnPrisoner();
			PrisonersSpawned++;
			SpawnTimer.Reset();
		}
		else
		{
			SpawnTimer.Tick(DeltaTime);
		}
	}
	else
	{
		// The round is complete
		GameMode->EndRound();
	}
}
