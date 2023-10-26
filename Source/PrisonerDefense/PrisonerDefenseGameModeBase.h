// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Round.h"

#include "PrisonerDefenseGameModeBase.generated.h"

/**
 * 
 */

UCLASS()
class PRISONERDEFENSE_API APrisonerDefenseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		APrisonerDefenseGameModeBase();

private:
	bool PrisonersShouldSpawn = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FGameRound> Rounds;

	TArray<class APDPrisonerCage*> PrisonerCages;

	int RoundIndex = 0;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool RoundIsRunning() const;

	UFUNCTION()
		void StartRound();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FGameRound GetCurrentRound() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		APDPrisonerCage* GetRandomPrisonerCage() const;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaTime) override;
};
