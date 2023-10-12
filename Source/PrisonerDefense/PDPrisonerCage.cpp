// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPrisonerCage.h"
#include "PDPrisoner.h"

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
	
}

// Called every frame
void APDPrisonerCage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PrisonersSpawned.Num() < PrisonersToSpawn)
	{
		SpawnTimer.Update(DeltaTime);
		if (SpawnTimer.OutOfTime())
		{
			APDPrisoner* prisoner = GetWorld()->SpawnActor<APDPrisoner>(PrisonerReference);
			prisoner->SetActorLocation(GetActorLocation());
			prisoner->Cage = this;

			PrisonersSpawned.Add(prisoner);

			SpawnTimer.Reset();
		}
	}
}

TArray<FVector> APDPrisonerCage::GetPatrolPoints() const
{
	return PatrolPoints;
}

