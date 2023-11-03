// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUtils.h"
#include "PDPlayer.h"
#include "Kismet/GameplayStatics.h"

APDPlayer* UCustomUtils::GetWorldPlayer(const UObject* WorldContextObject, const int32 PlayerIndex)
{
    return Cast<APDPlayer>(UGameplayStatics::GetPlayerPawn(WorldContextObject, PlayerIndex));
}
