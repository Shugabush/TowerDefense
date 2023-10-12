// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerDefenseGameModeBase.h"

#include "PDPlayer.h"

APrisonerDefenseGameModeBase::APrisonerDefenseGameModeBase()
{
	DefaultPawnClass = APDPlayer::StaticClass();
}

