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

int FPowerGeneratorUpgrade::GetNewPowerPerSecond() const
{
	return NewPowerPerSecond;
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

// Called when the game starts or when spawned
void APDPowerGenerator::BeginPlay()
{
	Super::BeginPlay();

	Timer = FCooldownTimer(1.0f / PowerPerSecond);

	TArray<int> UpgradeCosts;
	for (FPowerGeneratorUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}
	UpgradesWidget->InitializeUpgradeCosts(UpgradeCosts);
}

// Called every frame
void APDPowerGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If parent slot is not nullptr, then we've actually placed this power generator down
	if (Player->GetGameMode()->RoundIsRunning() && ParentSlot != nullptr)
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

void APDPowerGenerator::OnTowerPlaced()
{
	Widget->SetVisibility(true);
}

void APDPowerGenerator::Upgrade()
{
	FPowerGeneratorUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade)) return;

	PowerPerSecond = Upgrade.GetNewPowerPerSecond();
	CurrentUpgradeIndex++;
}

bool APDPowerGenerator::TryGetCurrentUpgrade(FPowerGeneratorUpgrade& Upgrade) const
{
	if (!Upgrades.IsValidIndex(CurrentUpgradeIndex)) { return false; }
	
	Upgrade = Upgrades[CurrentUpgradeIndex];
	return true;
}

