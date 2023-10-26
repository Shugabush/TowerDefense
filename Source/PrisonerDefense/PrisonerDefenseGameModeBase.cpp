// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerDefenseGameModeBase.h"

#include "PDPlayer.h"
#include "PDHUD.h"
#include "Round.h"
#include "PDPrisonerCage.h"
#include "Kismet/GameplayStatics.h"

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

void APrisonerDefenseGameModeBase::StartRound()
{
	PrisonersShouldSpawn = true;
}

FGameRound APrisonerDefenseGameModeBase::GetCurrentRound() const
{
	return Rounds[RoundIndex];
}

APDPrisonerCage* APrisonerDefenseGameModeBase::GetRandomPrisonerCage() const
{
	int RandomIndex = FMath::RandRange(0, PrisonerCages.Num());
	return PrisonerCages[RandomIndex];
}

void APrisonerDefenseGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Find prisoner cages
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APDPrisonerCage::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		APDPrisonerCage* Cage = Cast<APDPrisonerCage>(Actor);
		PrisonerCages.Add(Cage);
	}
	
	// Initialize rounds
	for (auto Round : Rounds)
	{
		Round.Initialize(this);
	}
}

void APrisonerDefenseGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RoundIsRunning())
	{
		for (FGameRound Round : Rounds)
		{
			Round.Tick(DeltaTime);
		}
	}
}

