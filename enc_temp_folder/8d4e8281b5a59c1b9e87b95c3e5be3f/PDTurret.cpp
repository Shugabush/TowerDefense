// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurret.h"
#include "PDTurretSlot.h"
#include "PDPrisoner.h"
#include "PDBullet.h"
#include "PDUpgradesWidget.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/KismetMathLibrary.h"

int FTurretUpgrade::GetPowerCost() const
{
	return PowerCost;
}

FCooldownTimer FTurretUpgrade::GetNewBulletSpawnTimer() const
{
	return NewBulletSpawnTimer;
}

// Sets default values
APDTurret::APDTurret() : APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticles"));
	FireParticles->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APDTurret::BeginPlay()
{
	Super::BeginPlay();

	TArray<int> UpgradeCosts;
	for (FTurretUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}
	UpgradesWidget->InitializeUpgradeCosts(UpgradeCosts);
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
		
		if (LookAtTarget != nullptr)
		{
			if (BulletSpawnTimer.OutOfTime())
			{
				float forwardDirectionDotProduct = FVector::DotProduct(GetActorForwardVector(), LookAtTarget->GetActorForwardVector());
				float lengthProduct = GetActorForwardVector().Size() * LookAtTarget->GetActorForwardVector().Size();

				float angle = FMath::Acos(forwardDirectionDotProduct / lengthProduct);

				if (angle < 5.f)
				{
					// Don't spawn any bullet, just damage the prisoner
					LookAtTarget->Damage(1);
					FireParticles->Activate();

					BulletSpawnTimer.Reset();
				}
			}
			else
			{
				BulletSpawnTimer.Tick(DeltaTime);
			}

			TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtTarget->GetActorLocation()).Quaternion();

			TargetRotation.X = 0;
			TargetRotation.Y = 0;

			SetActorRotation(FQuat::FastLerp(GetActorRotation().Quaternion(), TargetRotation, DeltaTime * RotationLerpSpeed));
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

void APDTurret::OnTowerPlaced()
{
	Super::OnTowerPlaced();
	Widget->SetVisibility(true);
}

void APDTurret::Upgrade()
{
	FTurretUpgrade Upgrade;
	if (!TryGetCurrentUpgrade(Upgrade)) return;

	BulletSpawnTimer.TimeLimit = Upgrade.GetNewBulletSpawnTimer().TimeLimit;

	CurrentUpgradeIndex++;
}

APDPrisoner* APDTurret::GetClosestTarget() const
{
	float closestDst = 10000;
	int targetIndex = -1;

	for (size_t i = 0; i < LookAtTargets.Num(); i++)
	{
		APDPrisoner* target = LookAtTargets[i];

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
