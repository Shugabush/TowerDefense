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
private:
	static TMap<UObject*, TArray<class APDPlayer*>> Players;
public:
	UFUNCTION()
		static APDPlayer* GetWorldPlayer(UObject* const WorldContextObject, const int32 PlayerIndex);

	UFUNCTION()
		static void Round(float& Value, const int DecimalPlaces);

	UFUNCTION()
		static FString SanitizeFloat(float Value, const int MaxDecimalPlaces = 0, const int MinDecimalPlaces = 0);
};
