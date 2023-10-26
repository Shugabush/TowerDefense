// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerDefenseGameModeBase.h"

#include "PDPlayer.h"
#include "PDHUD.h"

#include "UObject/ConstructorHelpers.h"

APrisonerDefenseGameModeBase::APrisonerDefenseGameModeBase()
{
	DefaultPawnClass = APDPlayer::StaticClass();
	HUDClass = APDHUD::StaticClass();
}

