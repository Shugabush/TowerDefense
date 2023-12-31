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

APDPrisoner* APDPrisonerCage::SpawnPrisoner()
{
	APDPrisoner* Prisoner = GetWorld()->SpawnActor<APDPrisoner>(PrisonerReference);
	Prisoner->SetActorLocation(GetActorLocation());
	Prisoner->Cage = this;
	return Prisoner;
}

