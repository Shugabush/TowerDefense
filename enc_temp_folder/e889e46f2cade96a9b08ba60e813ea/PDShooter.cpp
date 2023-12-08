// Fill out your copyright notice in the Description page of Project Settings.


#include "PDShooter.h"
#include "PDTurretSlot.h"
#include "PDPrisoner.h"
#include "PDBullet.h"
#include "PDUpgradesWidget.h"
#include "CustomUtils.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/KismetMathLibrary.h"

int FShooterUpgrade::GetPowerCost() const
{
	return PowerCost;
}

float FShooterUpgrade::GetAdditionalRangeScale() const
{
	return AdditionalRangeScale;
}

float FShooterUpgrade::GetAttackDamageMultiplier() const
{
	return AttackDamageMultiplier;
}

// Sets default values
APDShooter::APDShooter() : APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticles"));
	FireParticles->AttachTo(RootComponent);
}

FText APDShooter::GetUpgradeDescription() const
{
	FShooterUpgrade Upgrade;
	FString Description = "";

	float NewDamagePerSecond = DamageRate * Upgrade.GetAttackDamageMultiplier();

	if (TryGetCurrentUpgrade(Upgrade))
	{
		UCustomUtils::Round(NewDamagePerSecond, 3);

		Description = "Shoots " + UCustomUtils::SanitizeFloat(NewDamagePerSecond, 3) + " times per second";
	}
	return FText::FromString("Upgrade\n" + Description);
}

FText APDShooter::GetCurrentDescription() const
{
	float CurrentDamagePerSecond = DamageRate;
	UCustomUtils::Round(CurrentDamagePerSecond, 3);

	FString Description = "Shoots " + UCustomUtils::SanitizeFloat(CurrentDamagePerSecond, 3, 1) + " times per second";
	if (IsMaxLevel())
	{
		Description.InsertAt(0, "Max Level\n");
	}
	return FText::FromString("Current\n" + Description);
}

// Called when the game starts or when spawned
void APDShooter::BeginPlay()
{
	Super::BeginPlay();

	for (const FShooterUpgrade& Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}

	FireParticles->Deactivate();
}

void APDShooter::OnRoundEnded()
{
	DamageRate *= DamageMultiplierPerRound;

	Super::OnRoundEnded();
}

void APDShooter::OnFire(APDPrisoner* LookTarget, const float TargetDamage)
{
	RecieveOnFire(LookTarget, TargetDamage);
}

// Called every frame
void APDShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If parent slot is not nullptr, then we've actually placed this turret down
	if (ParentSlot != nullptr)
	{
		// Look at the closest object in the look at target list
		APDPrisoner* LookAtTarget = GetClosestTarget();

		bool ShouldActivateParticles = false;
		if (LookAtTarget != nullptr)
		{
			// We may have to damage the prisoner more than once on a given frame
			//LookAtTarget->Damage(DamagePerSecond * DeltaTime);

			OnFire(LookAtTarget, DamageRate * DeltaTime);
			ShouldActivateParticles = true;

			TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtTarget->GetActorLocation()).Quaternion();

			TargetRotation.X = 0;
			TargetRotation.Y = 0;

			Mesh->SetWorldRotation(TargetRotation);
		}

		if (ParticlesActivated)
		{
			ParticleTimer += DeltaTime;
		}

		if (ShouldActivateParticles && !ParticlesActivated)
		{
			FireParticles->Activate();
			ParticlesActivated = true;
		}
		else if (ParticlesActivated && ParticleTimer >= MinParticlePlayTime)
		{
			FireParticles->Deactivate();
			ParticlesActivated = false;
			ParticleTimer = 0;
		}
	}
}

void APDShooter::Upgrade()
{
	FShooterUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade)) return;

	DamageRate *= Upgrade.GetAttackDamageMultiplier();
	VolumeTriggerRadius = VolumeTrigger->GetUnscaledSphereRadius();
	RangeIndicatorScale = RangeIndicator->GetRelativeScale3D();

	VolumeTrigger->SetSphereRadius(VolumeTriggerRadius + Upgrade.GetAdditionalRangeScale());
	RangeIndicator->SetRelativeScale3D(RangeIndicatorScale + FVector(Upgrade.GetAdditionalRangeScale(), Upgrade.GetAdditionalRangeScale(), 0));
	
	Super::Upgrade();
}

APDPrisoner* APDShooter::GetClosestTarget() const
{
	float closestDst = 10000;

	APDPrisoner* closestTarget = nullptr;

	for (auto target : LookAtTargets)
	{	
		// Don't use any defeated prisoners as look targets
		if (target->IsDefeated())
		{
			continue;
		}

		float dstToTarget = FVector::Dist(GetActorLocation(), target->GetActorLocation());
		if (dstToTarget < closestDst)
		{
			closestDst = dstToTarget;
			closestTarget = target;
		}
	}
	return closestTarget;
}

void APDShooter::OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		LookAtTargets.Add(prisoner);
	}
}

void APDShooter::OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		LookAtTargets.Remove(prisoner);
	}
}

bool APDShooter::TryGetCurrentUpgrade(FShooterUpgrade& Upgrade) const
{
	if (IsMaxLevel()) return false;

	Upgrade = Upgrades[CurrentUpgradeIndex];

	return true;
}

bool APDShooter::IsMaxLevel() const
{
	return !Upgrades.IsValidIndex(CurrentUpgradeIndex);
}
