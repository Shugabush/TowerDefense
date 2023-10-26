// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPrisoner.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PDPrisonerCage.h"
#include "Kismet/KismetMathLibrary.h"

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

	bool gotTargetPoint = TryGetTargetPoint(targetPatrolPoint);
	bool gotPreviousPoint = TryGetPreviousPoint(previousPatrolPoint);

	if (Cage != nullptr && gotTargetPoint && TryGetPreviousPoint(previousPatrolPoint))
	{
		FVector lerpedPosition;

		targetPatrolPoint.Z = GetActorLocation().Z;
		previousPatrolPoint.Z = GetActorLocation().Z;

		float dstToPosition = FVector::Distance(GetActorLocation(), targetPatrolPoint);

		// Normalize the position offset for movement
		Velocity = targetPatrolPoint - previousPatrolPoint;
		Velocity.Normalize();
		Velocity *= MovementSpeed;

		if (dstToPosition > 0.1f)
		{
			lerpedPosition = GetActorLocation() + (Velocity * DeltaTime * 60);
		}
		else
		{
			TargetIndex++;
			lerpedPosition = targetPatrolPoint;
		}

		lerpedPosition.Z = GetActorLocation().Z;
		
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), lerpedPosition).Quaternion();

		SetActorRotation(FQuat::FastLerp(GetActorRotation().Quaternion(), TargetRotation, DeltaTime * RotationSpeed));

		SetActorLocation(lerpedPosition);
	}
	else
	{
		// Notify the user about any unexpected problems
		if (Cage == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(0, 0.25f, FColor::Red, GetName() + TEXT(" has no cage!"));
		}
		if (!gotTargetPoint)
		{
			GEngine->AddOnScreenDebugMessage(0, 0.25f, FColor::Red, GetName() + TEXT(" couldn't get their target patrol point!"));
		}
		if (!gotPreviousPoint)
		{
			GEngine->AddOnScreenDebugMessage(0, 0.25f, FColor::Red, GetName() + TEXT(" couldn't get their previous patrol point!"));
		}
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

