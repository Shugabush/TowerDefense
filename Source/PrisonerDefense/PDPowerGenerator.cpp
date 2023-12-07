// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPowerGenerator.h"

#include "PDPlayer.h"
#include "PDUserWidget.h"
#include "PDUpgradesWidget.h"
#include "PrisonerDefenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "CustomUtils.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"

float FPowerGeneratorUpgrade::GetPowerCost() const
{
	return PowerCost;
}

float FPowerGeneratorUpgrade::GetAdditionalPowerPerSecond() const
{
	return AdditionalPowerPerSecond;
}


// Sets default values
APDPowerGenerator::APDPowerGenerator() : APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GenerationParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GenerationParticles"));
	GenerationParticles->AttachTo(RootComponent);
}

UStaticMeshComponent* APDPowerGenerator::GetMesh() const
{
	return Mesh;
}

FText APDPowerGenerator::GetUpgradeDescription() const
{
	FString Description = "";
	FPowerGeneratorUpgrade Upgrade;
	if (TryGetCurrentUpgrade(Upgrade))
	{
		Description = UCustomUtils::SanitizeFloat(PowerPerSecond + Upgrade.GetAdditionalPowerPerSecond(), 2, 2) + " power per second";
	}
	return FText::FromString(Description);
}

FText APDPowerGenerator::GetCurrentDescription() const
{
	FString Description = UCustomUtils::SanitizeFloat(PowerPerSecond, 2, 2) + " power per second";
	if (IsMaxLevel())
	{
		Description.InsertAt(0, "Max Level\n");
	}
	return FText::FromString(Description);
}

// Called when the game starts or when spawned
void APDPowerGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (FPowerGeneratorUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}
}

void APDPowerGenerator::OnRoundStarted()
{
	GenerationParticles->SetActive(true);

	Super::OnRoundStarted();
}

void APDPowerGenerator::OnRoundEnded()
{
	PowerPerSecond *= PowerPerSecondMultiplierPerRound;

	GenerationParticles->SetActive(false);

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

		Player->UpdatePower(PowerPerSecond / Fps);
	}
}

void APDPowerGenerator::Upgrade()
{
	FPowerGeneratorUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade)) return;

	PowerPerSecond += Upgrade.GetAdditionalPowerPerSecond();
	
	Super::Upgrade();
}

void APDPowerGenerator::OnTowerPlaced()
{
	if (Player != nullptr && Player->GetGameMode() != nullptr)
	{
		GenerationParticles->SetActive(Player->GetGameMode()->RoundIsRunning());
	}
}

bool APDPowerGenerator::TryGetCurrentUpgrade(FPowerGeneratorUpgrade& Upgrade) const
{
	if (IsMaxLevel()) return false;
	
	Upgrade = Upgrades[CurrentUpgradeIndex];
	return true;
}

bool APDPowerGenerator::IsMaxLevel() const
{
	return !Upgrades.IsValidIndex(CurrentUpgradeIndex);
}

