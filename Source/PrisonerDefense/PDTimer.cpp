// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTimer.h"

FPDTimer::FPDTimer()
{
	TimeElapsed = 0;
	TimeLimit = 1;
}

FPDTimer::FPDTimer(float timeLimit)
{
	TimeElapsed = 0;
	TimeLimit = timeLimit;
}

FPDTimer::~FPDTimer()
{

}

void FPDTimer::Update(float time)
{
	TimeElapsed += time;
	TimeElapsed = FMath::Clamp<float>(TimeElapsed, 0, TimeLimit);
}

void FPDTimer::Reset()
{
	TimeElapsed = 0;
}

bool FPDTimer::OutOfTime() const
{
	return TimeElapsed >= TimeLimit;
}

float FPDTimer::GetTimeRemaining() const
{
	return TimeLimit - TimeElapsed;
}

float FPDTimer::GetFractionOfTimeElapsed() const
{
	return TimeElapsed / TimeLimit;
}

float FPDTimer::GetFractionOfTimeRemaining() const
{
	return GetTimeRemaining() / TimeLimit;
}
