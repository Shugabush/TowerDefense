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

	class APDHUD* HUD;
	class UPDUserWidget* Widget;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool RoundIsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class APDPlayer* GetPlayer() const;

	FOnRoundChangedSignature OnRoundChanged;
	FOnRoundStartedSignature OnRoundStarted;
	FOnRoundStartedSignature OnRoundEnded;

	int GetNewMaxHealth() const;

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
};
