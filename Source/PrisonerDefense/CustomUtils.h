// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomUtils.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UCustomUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		static class APDPlayer* GetWorldPlayer(const UObject* WorldContextObject, const int32 PlayerIndex);
};
