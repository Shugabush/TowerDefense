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

FGameRound& APrisonerDefenseGameModeBase::GetCurrentRound()
{
	return Rounds[RoundIndex];
}

APDPrisonerCage* APrisonerDefenseGameModeBase::GetRandomPrisonerCage() const
{
	int RandomIndex = FMath::RandRange(0, PrisonerCages.Num() - 1);
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
	for (size_t i = 0; i < Rounds.Num(); i++)
	{
		Rounds[i].Initialize(this);
	}
}

void APrisonerDefenseGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RoundIsRunning())
	{
		for (size_t i = 0; i < Rounds.Num(); i++)
		{
			Rounds[i].Tick(DeltaTime);
		}
	}
}

