// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurret.h"
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

int FTurretUpgrade::GetPowerCost() const
{
	return PowerCost;
}

float FTurretUpgrade::GetAdditionalRangeScale() const
{
	return AdditionalRangeScale;
}

float FTurretUpgrade::GetAttackCooldownMultiplier() const
{
	return AttackCooldownMultiplier;
}

// Sets default values
APDTurret::APDTurret() : APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticles"));
	FireParticles->AttachTo(RootComponent);
}

FText APDTurret::GetUpgradeDescription() const
{
	FTurretUpgrade Upgrade;

	float CurrentCooldown = AttackCooldown.TimeLimit;
	FString Description = "Max Level\nShoots every " + UCustomUtils::SanitizeFloat(CurrentCooldown, 3, 1) + " seconds";

	if (TryGetCurrentUpgrade(Upgrade))
	{
		float NewCooldown = CurrentCooldown * Upgrade.GetAttackCooldownMultiplier();

		UCustomUtils::Round(NewCooldown, 3);

		float NewCooldownReciprocal = 1.f / NewCooldown;
		UCustomUtils::Round(NewCooldownReciprocal, 3);

		Description = "Shoots every " + UCustomUtils::SanitizeFloat(CurrentCooldown, 3, 1) + "->" +
			UCustomUtils::SanitizeFloat(NewCooldown, 3) + " seconds" + "\n (" + UCustomUtils::SanitizeFloat(NewCooldownReciprocal, 3) + " times per second)";
	}
	return FText::FromString(Description);
}

// Called when the game starts or when spawned
void APDTurret::BeginPlay()
{
	Super::BeginPlay();

	for (FTurretUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}

	FireParticles->Deactivate();
}

void APDTurret::OnRoundEnded()
{
	AttackCooldown.TimeLimit *= CooldownMultiplierPerRound;

	Super::OnRoundEnded();
}

// Called every frame
void APDTurret::Tick(float DeltaTime)
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
			float ForwardDirectionDotProduct = FVector::DotProduct(GetActorForwardVector(), LookAtTarget->GetActorForwardVector());
			float LengthProduct = GetActorForwardVector().Size() * LookAtTarget->GetActorForwardVector().Size();

			float LookAngle = FMath::Acos(ForwardDirectionDotProduct / LengthProduct);

			if (AttackCooldown.TimeLimit <= DeltaTime)
			{
				// We may have to damage the prisoner more than once on a given frame
				if (LookAngle < 5.f)
				{
					// Damage the prisoner
					LookAtTarget->Damage(DeltaTime / AttackCooldown.TimeLimit);

					ShouldActivateParticles = true;
				}
			}
			else
			{
				if (AttackCooldown.OutOfTime())
				{
					if (LookAngle < 5.f)
					{
						// Damage the prisoner
						LookAtTarget->Damage(1);
						
						ShouldActivateParticles = true;

						AttackCooldown.Reset();
					}
				}
				else
				{
					AttackCooldown.Tick(DeltaTime);
				}
			}

			TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtTarget->GetActorLocation()).Quaternion();

			TargetRotation.X = 0;
			TargetRotation.Y = 0;

			SetActorRotation(FQuat::FastLerp(GetActorRotation().Quaternion(), TargetRotation, DeltaTime * RotationLerpSpeed));
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

void APDTurret::BlendMeshColors(FLinearColor newColor)
{
	MeshData.BlendColors(newColor);
}

void APDTurret::SetMeshColors(FLinearColor newColor)
{
	MeshData.SetColors(newColor);
}

void APDTurret::ResetMeshColors()
{
	MeshData.ResetColors();
}

void APDTurret::Upgrade()
{
	FTurretUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade)) return;

	AttackCooldown.TimeLimit *= Upgrade.GetAttackCooldownMultiplier();
	VolumeTriggerRadius = VolumeTrigger->GetUnscaledSphereRadius();
	RangeIndicatorScale = RangeIndicator->GetRelativeScale3D();

	VolumeTrigger->SetSphereRadius(VolumeTriggerRadius + Upgrade.GetAdditionalRangeScale());
	RangeIndicator->SetRelativeScale3D(RangeIndicatorScale + FVector(Upgrade.GetAdditionalRangeScale(), Upgrade.GetAdditionalRangeScale(), 0));
	
	Super::Upgrade();
}

APDPrisoner* APDTurret::GetClosestTarget() const
{
	float closestDst = 10000;
	int targetIndex = -1;

	for (size_t i = 0; i < LookAtTargets.Num(); i++)
	{
		APDPrisoner* target = LookAtTargets[i];
		
		// Don't use any defeated prisoners as look targets
		if (target->IsDefeated())
		{
			continue;
		}

		float dstToTarget = FVector::Dist(GetActorLocation(), target->GetActorLocation());
		if (dstToTarget < closestDst)
		{
			closestDst = dstToTarget;
			targetIndex = i;
		}
	}
	if (LookAtTargets.IsValidIndex(targetIndex))
	{
		return LookAtTargets[targetIndex];
	}
	else
	{
		return nullptr;
	}
}

void APDTurret::OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		LookAtTargets.Add(prisoner);
	}
}

void APDTurret::OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		LookAtTargets.Remove(prisoner);
	}
}

bool APDTurret::TryGetCurrentUpgrade(FTurretUpgrade& Upgrade) const
{
	if (!Upgrades.IsValidIndex(CurrentUpgradeIndex)) { return false; }

	Upgrade = Upgrades[CurrentUpgradeIndex];

	return true;
}
