// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool RoundIsRunning() const;

	UFUNCTION()
		void StartRound();
};
