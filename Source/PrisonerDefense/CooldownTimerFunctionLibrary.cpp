// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownTimerFunctionLibrary.h"

void UCooldownTimerFunctionLibrary::TickCooldownTimer(UPARAM(ref)FCooldownTimer& timer, float time)
{
	timer.Tick(time);
}

void UCooldownTimerFunctionLibrary::ResetCooldownTimer(UPARAM(ref)FCooldownTimer& timer)
{
	timer.Reset();
}

bool UCooldownTimerFunctionLibrary::CooldownTimerOutOfTime(UPARAM(ref)const FCooldownTimer& timer)
{
	return timer.OutOfTime();
}
