// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPrisoner.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PDPrisonerCage.h"

// Sets default values
APDPrisoner::APDPrisoner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(Capsule);

	RootComponent = Capsule;
}

// Called when the game starts or when spawned
void APDPrisoner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APDPrisoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector targetPatrolPoint;
	FVector previousPatrolPoint;

	if (TargetIndex <= 0)
	{
		TargetIndex = 1;
	}

	if (Cage != nullptr && TryGetTargetPoint(targetPatrolPoint) && TryGetPreviousPoint(previousPatrolPoint))
	{
		FVector lerpedPosition;

		float dstToPosition = FVector::Distance(GetActorLocation(), targetPatrolPoint);

		// Normalize the position offset for movement
		FVector lerpDirection = targetPatrolPoint - previousPatrolPoint;
		lerpDirection.Normalize();

		if (dstToPosition > 0.1f)
		{
			lerpedPosition = GetActorLocation() + (lerpDirection * MovementSpeed);
		}
		else
		{
			TargetIndex++;
			lerpedPosition = targetPatrolPoint;
		}

		lerpedPosition.Z = GetActorLocation().Z;

		SetActorLocation(lerpedPosition);
	}
}

FVector APDPrisoner::GetGroundVelocity() const
{
	return Velocity;
}

bool APDPrisoner::TryGetTargetPoint(FVector& TargetPoint) const
{
	return Cage->TryGetPatrolPoint(TargetIndex, TargetPoint);
}

bool APDPrisoner::TryGetPreviousPoint(FVector& PreviousPoint) const
{
	return Cage->TryGetPatrolPoint(TargetIndex - 1, PreviousPoint);
}

