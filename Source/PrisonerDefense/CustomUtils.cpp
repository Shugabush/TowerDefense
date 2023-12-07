// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUtils.h"
#include "PDPlayer.h"
#include "Kismet/GameplayStatics.h"
#include <cassert>

TMap<UObject*, TArray<class APDPlayer*>> UCustomUtils::Players;

APDPlayer* UCustomUtils::GetWorldPlayer(UObject* const WorldContextObject, const int32 PlayerIndex)
{
    TArray<APDPlayer*> PlayerArray = Players.FindOrAdd(WorldContextObject, TArray<APDPlayer*>());

    APDPlayer* Player = nullptr;

    if (PlayerArray.Num() <= PlayerIndex)
    {
        Player = Cast<APDPlayer>(UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex));
        PlayerArray.EmplaceAt(PlayerIndex, Player);
    }
    else
    {
        Player = PlayerArray[PlayerIndex];
    }
    // Update the player array in the map
    Players[WorldContextObject] = PlayerArray;
    return Player;
}

void UCustomUtils::Round(float& Value, const int DecimalPlaces)
{
    int ExpMultiplier = FMath::Pow(10, DecimalPlaces);

    float ExpValue = FMath::RoundToFloat(Value * ExpMultiplier);
    Value = ExpValue / ExpMultiplier;
}

FString UCustomUtils::SanitizeFloat(float Value, const int MaxDecimalPlaces, const int MinDecimalPlaces)
{
    if (!ensure(MaxDecimalPlaces >= MinDecimalPlaces))
    {
        GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("The minimum decimal places cannot be greater than the maximum decimal places!"));
    }

    UCustomUtils::Round(Value, MaxDecimalPlaces);

    int IntValue = (int)Value;
    float FloatValue = Value - IntValue;

    FString Str = FString::FromInt(IntValue);

    if (MaxDecimalPlaces > 0)
    {
        Str += ".";
    }

    int CurrentDecimal;

    for (CurrentDecimal = 0; CurrentDecimal < MaxDecimalPlaces; CurrentDecimal++)
    {
        FloatValue *= 10;
        int ModuloValue = (int)FloatValue % 10;
        Str += FString::FromInt(ModuloValue);
    }

    while (Str.EndsWith("0") && CurrentDecimal > MinDecimalPlaces)
    {
        Str.RemoveFromEnd("0");
        CurrentDecimal--;
    }
    if (Str.EndsWith("."))
    {
        Str.RemoveFromEnd(".");
    }

    return Str;
}
