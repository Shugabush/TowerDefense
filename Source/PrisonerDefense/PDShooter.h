// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"
#include "PDTower.h"

#include "GameFramework/Actor.h"
#include "PDShooter.generated.h"

USTRUCT(BlueprintType)
// Turret upgrade blueprint
struct FShooterUpgrade
{
	GENERATED_BODY()
public:
	int GetPowerCost() const;
	float GetAdditionalRangeScale() const;
	float GetAttackDamageMultiplier() const;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerCost = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float AdditionalRangeScale = 0.1f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float AttackDamageMultiplier = 2.f;
};

UCLASS()
class PRISONERDEFENSE_API APDShooter : public APDTower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDShooter();

	virtual FText GetUpgradeDescription() const override;
	virtual FText GetCurrentDescription() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnRoundEnded() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float RotationLerpSpeed = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		// Attack cooldown's timer will multiply by this amount after each round
		// Set to 1 if you don't want that effect
		float DamageMultiplierPerRound = 0.9f;

	UFUNCTION()
		virtual void OnFire(class APDPrisoner* LookTarget, const float TargetDamage);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFire"))
		void RecieveOnFire(class APDPrisoner* LookTarget, const float TargetDamage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Upgrade() override;

	UPROPERTY(BlueprintReadOnly)
		FQuat TargetRotation;

	virtual void OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		float DamageRate = 1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FShooterUpgrade> Upgrades;

	UPROPERTY()
		TSet<APDPrisoner*> LookAtTargets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		// Particles to play when this turret fires
		UParticleSystemComponent* FireParticles;

	UFUNCTION()
		class APDPrisoner* GetClosestTarget() const;

	bool TryGetCurrentUpgrade(FShooterUpgrade& Upgrade) const;

	bool IsMaxLevel() const;
	
	// Will not rely on any bool inside the particle system itself for activated
	bool ParticlesActivated;

	float ParticleTimer;

	const float MinParticlePlayTime = 0.25f;
};
