// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"
#include "PDTower.h"

#include "GameFramework/Actor.h"
#include "PDTurret.generated.h"

USTRUCT(BlueprintType)
// Turret upgrade blueprint
struct FTurretUpgrade
{
	GENERATED_BODY()
public:
	int GetPowerCost() const;
	float GetAdditionalRangeScale() const;
	float GetAttackCooldownMultiplier() const;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerCost = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float AdditionalRangeScale = 0.1f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float AttackCooldownMultiplier = 0.5f;
};

UCLASS()
class PRISONERDEFENSE_API APDTurret : public APDTower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDTurret();

	virtual FText GetUpgradeDescription() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnRoundEnded() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float RotationLerpSpeed = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		// Attack cooldown's timer will multiply by this amount after each round
		// Set to 1 if you don't want that effect
		float CooldownMultiplierPerRound = 0.9f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BlendMeshColors(FLinearColor newColor);
	void SetMeshColors(FLinearColor newColor);
	void ResetMeshColors();

	virtual void Upgrade() override;

	FQuat TargetRotation;

	virtual void OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		FCooldownTimer AttackCooldown;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FTurretUpgrade> Upgrades;

	UPROPERTY()
		TArray<APDPrisoner*> LookAtTargets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		// Particles to play when this turret fires
		UParticleSystemComponent* FireParticles;

	UFUNCTION()
		class APDPrisoner* GetClosestTarget() const;

	bool TryGetCurrentUpgrade(FTurretUpgrade& Upgrade) const;
	
	// Will not rely on any bool inside the particle system itself for activated
	bool ParticlesActivated;
};
