// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"
#include "PDTower.h"

#include "GameFramework/Actor.h"
#include "PDPowerGenerator.generated.h"

USTRUCT(BlueprintType)
// Power Generator upgrade blueprint
struct FPowerGeneratorUpgrade
{
	GENERATED_BODY()
public:
	int GetPowerCost() const;
	int GetAdditionalPowerPerSecond() const;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerCost = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		int AdditionalPowerPerSecond = 5;
};

UCLASS()
class PRISONERDEFENSE_API APDPowerGenerator : public APDTower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDPowerGenerator();

	class UStaticMeshComponent* GetMesh() const;

	virtual FText GetUpgradeDescription() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnRoundEnded() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Upgrade() override;
	
private:
	FCooldownTimer Timer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FPowerGeneratorUpgrade> Upgrades;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float PowerPerSecond = 2;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float PowerPerSecondMultiplierPerRound = 1.25f;

	bool TryGetCurrentUpgrade(FPowerGeneratorUpgrade& Upgrade) const;
};
