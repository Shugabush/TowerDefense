// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CooldownTimer.h"

#include "CooldownTimerFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UCooldownTimerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Cooldown Timer")
	static void TickCooldownTimer(UPARAM(ref)FCooldownTimer& timer, float time);

	UFUNCTION(BlueprintCallable, Category = "Cooldown Timer")
	static void ResetCooldownTimer(UPARAM(ref)FCooldownTimer& timer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cooldown Timer")
	static bool CooldownTimerOutOfTime(UPARAM(ref)const FCooldownTimer& timer);
	
};
