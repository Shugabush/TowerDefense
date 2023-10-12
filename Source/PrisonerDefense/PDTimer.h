// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PDTimer.generated.h"

/**
 * 
 */
USTRUCT()
struct PRISONERDEFENSE_API FPDTimer
{
	GENERATED_BODY()
private:
	float TimeElapsed;
	float TimeLimit;
public:
	FPDTimer();
	FPDTimer(float timeLimit);
	~FPDTimer();

	void Update(float time);
	void Reset();
	bool OutOfTime() const;

	float GetTimeRemaining() const;

	float GetFractionOfTimeElapsed() const;
	float GetFractionOfTimeRemaining() const;
};
