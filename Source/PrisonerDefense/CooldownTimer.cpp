// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownTimer.h"

FCooldownTimer::FCooldownTimer()
{
	TimeElapsed = 0;
	TimeLimit = 1;
}

FCooldownTimer::FCooldownTimer(float timeLimit)
{
	TimeElapsed = 0;
	TimeLimit = timeLimit;
}

FCooldownTimer::~FCooldownTimer()
{
	
}

void FCooldownTimer::Tick(float time)
{
	TimeElapsed += time;

	TimeElapsed = FMath::Clamp<float>(TimeElapsed, 0, TimeLimit);
}

bool FCooldownTimer::OutOfTime() const
{
	return TimeElapsed >= TimeLimit;
}

void FCooldownTimer::Reset()
{
	TimeElapsed = 0;
}
