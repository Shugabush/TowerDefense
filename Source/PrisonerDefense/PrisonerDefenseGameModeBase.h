// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Round.h"

#include "PrisonerDefenseGameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundChangedSignature, int, NewRound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundEndedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictorySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLossSignature);

UCLASS()
class PRISONERDEFENSE_API APrisonerDefenseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		APrisonerDefenseGameModeBase();
private:
	bool PrisonersShouldSpawn = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FGameRound> Rounds;

	UPROPERTY()
		TArray<class APDPrisonerCage*> PrisonerCages;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true, ToolTip = "Starting round. Modify for testing purposes."))
		int RoundIndex = 0;

	UPROPERTY()
		class APDPlayer* Player;

	UPROPERTY()
		class APDHUD* HUD;

	UPROPERTY()
		class UPDUserWidget* Widget;

	bool GameRunning = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool RoundIsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GameIsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class APDPlayer* GetPlayer() const;

	FOnRoundChangedSignature OnRoundChanged;
	FOnRoundStartedSignature OnRoundStarted;
	FOnRoundStartedSignature OnRoundEnded;
	FOnVictorySignature OnVictory;
	FOnLossSignature OnLoss;

	int GetNewMaxHealth() const;
	int GetNewMovementSpeed() const;

	UFUNCTION()
		void StartRound();

	UFUNCTION()
		void EndRound();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetCurrentRoundNumber() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCurrentRound(FGameRound& Round) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		APDPrisonerCage* GetRandomPrisonerCage() const;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Victory();
	void Loss();
};
