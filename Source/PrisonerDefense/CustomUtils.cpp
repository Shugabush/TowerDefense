// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUtils.h"
#include "PDPlayer.h"
#include "Kismet/GameplayStatics.h"

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
