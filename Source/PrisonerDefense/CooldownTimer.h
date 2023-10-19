// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PRISONERDEFENSE_API FCooldownTimer
{
	GENERATED_BODY()

private:
	float TimeElapsed;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float TimeLimit;
public:
	FCooldownTimer();
	FCooldownTimer(float timeLimit);
	~FCooldownTimer();

	void Tick(float time);
	bool OutOfTime() const;
	void Reset();

	float GetTimeLimit() const;
	float GetTimeElapsed() const;
	float GetTimeRemaining() const;

	float GetFractionOfTimeElapsed() const;
	float GetFractionOfTimeRemaining() const;
};
