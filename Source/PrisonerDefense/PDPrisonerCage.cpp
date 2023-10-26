// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPrisonerCage.h"
#include "PDPrisoner.h"
#include "PrisonerDefenseGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APDPrisonerCage::APDPrisonerCage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APDPrisonerCage::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnTimer = FCooldownTimer(SpawnInterval);

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void APDPrisonerCage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameMode->RoundIsRunning() && PrisonersSpawned < PrisonersToSpawn)
	{
		SpawnTimer.Tick(DeltaTime);
		if (SpawnTimer.OutOfTime())
		{
			APDPrisoner* prisoner = GetWorld()->SpawnActor<APDPrisoner>(PrisonerReference);
			prisoner->SetActorLocation(GetActorLocation());
			prisoner->Cage = this;

			PrisonersSpawned++;

			SpawnTimer.Reset();
		}
	}
}

bool APDPrisonerCage::TryGetPatrolPoint(int index, FVector& patrolPoint) const
{
	if (index < 0)
	{
		patrolPoint = GetActorLocation();
		return true;
	}
	else if (index >= GetPatrolPointCount())
	{
		return false;
	}
	patrolPoint = PatrolPoints[index];
	return true;
}

int APDPrisonerCage::GetPatrolPointCount() const
{
	return PatrolPoints.Num();
}

