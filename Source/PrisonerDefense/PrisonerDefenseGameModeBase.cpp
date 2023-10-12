// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerDefenseGameModeBase.h"

#include "PDPlayer.h"
#include "UObject/ConstructorHelpers.h"

APrisonerDefenseGameModeBase::APrisonerDefenseGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerClass(TEXT("/Game/Blueprints/BP_PDPlayer"));
	DefaultPawnClass = PlayerClass.Class;
}

