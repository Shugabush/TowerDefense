// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CooldownTimer.h"

#include "Round.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PRISONERDEFENSE_API FGameRound
{
	GENERATED_BODY()

public:
	FGameRound() = default;

	void Initialize(class APrisonerDefenseGameModeBase* GameMode);

	int GetPowerReward() const;

	void Tick(float DeltaTime);
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		int PrisonersToSpawn = 5;

	int PrisonersSpawned = 0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float SpawnInterval = 1.25f;

	UPROPERTY()
		class APrisonerDefenseGameModeBase* GameMode;

	UPROPERTY()
		TArray<class APDPrisoner*> Prisoners;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerReward = 100;

	bool AllPrisonersDefeated() const;

	FCooldownTimer SpawnTimer;
};
