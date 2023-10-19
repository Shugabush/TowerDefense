// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurret.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APDTurret::APDTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh->AttachTo(RootComponent);
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
}

// Called every frame
void APDTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Look at the closest object in the look at target list
	AActor* LookAtTarget = GetClosestTarget();

	if (LookAtTarget != nullptr)
	{
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtTarget->GetActorLocation()).Quaternion();

		TargetRotation.X = 0;
		TargetRotation.Y = 0;

		SetActorRotation(FQuat::FastLerp(GetActorRotation().Quaternion(), TargetRotation, DeltaTime * RotationLerpSpeed));
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

AActor* APDTurret::GetClosestTarget() const
{
	float closestDst = 10000;
	int targetIndex = -1;

	for (size_t i = 0; i < LookAtTargets.Num(); i++)
	{
		AActor* target = LookAtTargets[i];

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

