// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"

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
	UFUNCTION(BlueprintCallable)
		bool TryGetPatrolPoint(int index, FVector& patrolPoint) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetPatrolPointCount() const;

	UFUNCTION(BlueprintCallable)
		class APDPrisoner* SpawnPrisoner();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDPrisoner> PrisonerReference;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FVector> PatrolPoints = TArray<FVector>();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float SpawnInterval = 2;

	UPROPERTY()
		class APrisonerDefenseGameModeBase* GameMode;

	FCooldownTimer SpawnTimer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		// How many prisoners should spawn from this cage?
		int PrisonersToSpawn = 5;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		// How many prisoners have been spawned from this cage?
		int PrisonersSpawned = 0;
};
