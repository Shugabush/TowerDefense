// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPowerGenerator.h"

#include "PDPlayer.h"
#include "PDUserWidget.h"
#include "PDUpgradesWidget.h"
#include "PrisonerDefenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

int FPowerGeneratorUpgrade::GetPowerCost() const
{
	return PowerCost;
}

int FPowerGeneratorUpgrade::GetAdditionalPowerPerSecond() const
{
	return AdditionalPowerPerSecond;
}


// Sets default values
APDPowerGenerator::APDPowerGenerator() : APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UStaticMeshComponent* APDPowerGenerator::GetMesh() const
{
	return Mesh;
}

FText APDPowerGenerator::GetUpgradeDescription() const
{
	FPowerGeneratorUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade))
	{
		return FText::FromString("Max Level\n " + FString::FromInt(PowerPerSecond) + " per second");
	}
	return FText::FromString(FString::FromInt(PowerPerSecond) + "->" + FString::FromInt(PowerPerSecond + Upgrade.GetAdditionalPowerPerSecond()) + " per second");
}

// Called when the game starts or when spawned
void APDPowerGenerator::BeginPlay()
{
	Super::BeginPlay();

	Timer = FCooldownTimer(1.0f / PowerPerSecond);

	for (FPowerGeneratorUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}
}

void APDPowerGenerator::OnRoundEnded()
{
	PowerPerSecond *= PowerPerSecondMultiplierPerRound;

	Super::OnRoundEnded();
}

// Called every frame
void APDPowerGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If parent slot is not nullptr, then we've actually placed this power generator down
	if (Player->GetGameMode()->RoundIsRunning() && ParentSlot != nullptr)
	{
		float Fps = 1.f / DeltaTime;

		if (PowerPerSecond > Fps)
		{
			// If power per second is more than fps, than we need to add more than one power on at least some frames
			Player->GetWidget()->UpdatePower(PowerPerSecond / Fps);
		}
		else
		{
			if (Timer.OutOfTime())
			{
				// Update power
				Player->GetWidget()->UpdatePower(1);
				Timer.Reset();
			}
			else
			{
				Timer.Tick(DeltaTime);
			}
		}
	}
}

void APDPowerGenerator::Upgrade()
{
	FPowerGeneratorUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade)) return;

	PowerPerSecond += Upgrade.GetAdditionalPowerPerSecond();
	Timer.TimeLimit = 1.f / PowerPerSecond;
	
	Super::Upgrade();
}

bool APDPowerGenerator::TryGetCurrentUpgrade(FPowerGeneratorUpgrade& Upgrade) const
{
	if (!Upgrades.IsValidIndex(CurrentUpgradeIndex)) { return false; }
	
	Upgrade = Upgrades[CurrentUpgradeIndex];
	return true;
}

