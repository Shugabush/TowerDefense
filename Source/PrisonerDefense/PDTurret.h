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
	FCooldownTimer GetNewAttackCooldown() const;
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerCost = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FCooldownTimer NewAttackCooldown;
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		float RotationLerpSpeed = 5;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDBullet> BulletBlueprint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FCooldownTimer AttackCooldown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FTurretUpgrade> Upgrades;

	UPROPERTY()
		TArray<APDPrisoner*> LookAtTargets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		// Particles to play when this turret fires
		UParticleSystemComponent* FireParticles;

	UFUNCTION()
		class APDPrisoner* GetClosestTarget() const;

	bool TryGetCurrentUpgrade(FTurretUpgrade& Upgrade) const;
};
