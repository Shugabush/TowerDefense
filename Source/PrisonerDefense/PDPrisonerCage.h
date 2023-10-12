// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PDTimer.h"

#include "GameFramework/Actor.h"
#include "PDPrisonerCage.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPrisonerCage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDPrisonerCage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get a patrol point at the given index
	bool TryGetPatrolPoint(int index, FVector& patrolPoint) const;

	int GetPatrolPointCount() const;

private:
	TArray<class APDPrisoner*> PrisonersSpawned;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDPrisoner> PrisonerReference;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FVector> PatrolPoints = TArray<FVector>();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float SpawnInterval = 2;

	FPDTimer SpawnTimer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		int PrisonersToSpawn = 5;
};
