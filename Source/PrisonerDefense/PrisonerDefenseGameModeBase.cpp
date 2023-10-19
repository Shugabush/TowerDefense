// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrisonerDefenseGameModeBase.h"

#include "PDHUD.h"
#include "UObject/ConstructorHelpers.h"

APrisonerDefenseGameModeBase::APrisonerDefenseGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerBlueprint(TEXT("/Game/Blueprints/BP_PDPlayer"));
	DefaultPawnClass = PlayerBlueprint.Class;

	static ConstructorHelpers::FClassFinder<AHUD> HUDBlueprint(TEXT("/Game/Blueprints/BP_PDHUD"));
	HUDClass = HUDBlueprint.Class;
}

