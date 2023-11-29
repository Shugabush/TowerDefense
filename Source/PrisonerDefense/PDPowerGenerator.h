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
	int GetNewPowerPerSecond() const;
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerCost = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		int NewPowerPerSecond;
};

UCLASS()
class PRISONERDEFENSE_API APDPowerGenerator : public APDTower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDPowerGenerator();

	class UStaticMeshComponent* GetMesh() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FText GetUpgradeDescription() const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnTowerPlaced() override;

	virtual void Upgrade() override;
	
private:
	FCooldownTimer Timer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FPowerGeneratorUpgrade> Upgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int PowerPerSecond = 2;

	bool TryGetCurrentUpgrade(FPowerGeneratorUpgrade& Upgrade) const;
};
