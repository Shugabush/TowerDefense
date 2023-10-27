// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurret.h"
#include "PDTurretSlot.h"
#include "PDPrisoner.h"
#include "PDBullet.h"

#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"

int FTurretUpgrade::GetPowerCost() const
{
	return PowerCost;
}

// Sets default values
APDTurret::APDTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh->AttachTo(RootComponent);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpgradesWidget"));
	Widget->AttachTo(RootComponent);
}

UStaticMeshComponent* APDTurret::GetMesh()
{
	return Mesh;
}

// Called when the game starts or when spawned
void APDTurret::BeginPlay()
{
	Super::BeginPlay();
	
	MeshData = MeshRenderData(Mesh, "Color");

	// Make the first bullet spawn immediately (once this turret has a prisoner to fire at)
	BulletSpawnTimer.Tick(BulletSpawnTimer.GetTimeLimit());

	Widget->SetVisibility(false);

	TArray<int> UpgradeCosts;
	for (FTurretUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}
}

// Called every frame
void APDTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If parent slot is not nullptr, then we've actually placed this turret down
	if (ParentSlot != nullptr)
	{
		// Look at the closest object in the look at target list
		APDPrisoner* LookAtTarget = ParentSlot->GetClosestTarget();

		if (LookAtTarget != nullptr)
		{
			if (BulletSpawnTimer.OutOfTime())
			{
				// Spawn a new bullet
				APDBullet* newBullet = GetWorld()->SpawnActor<APDBullet>(BulletBlueprint, FTransform(GetActorLocation()));
				if (newBullet != nullptr)
				{
					newBullet->TargetPrisoner = LookAtTarget;
				}

				BulletSpawnTimer.Reset();
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

void APDTurret::OnTurretPlaced()
{
	Widget->SetVisibility(true);
}
