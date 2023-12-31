// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPrisoner.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "PDPrisonerCage.h"
#include "PDPlayer.h"
#include "PDUserWidget.h"
#include "PDHealthWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PrisonerDefenseGameModeBase.h"

// Sets default values
APDPrisoner::APDPrisoner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Capsule);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	Widget->AttachTo(Capsule);

	RootComponent = Capsule;
}

void APDPrisoner::Damage(const float Damage)
{
	Health -= Damage;
	HealthWidget->SetMeterFill(Health / MaxHealth);
	if (Player != nullptr)
	{
		Player->UpdatePower(PowerReward * Damage);
	}
	if (Health <= 0)
	{
		OnDefeat();
	}
}

void APDPrisoner::OnDefeat()
{
	Defeated = true;
}

// Called when the game starts or when spawned
void APDPrisoner::BeginPlay()
{
	Super::BeginPlay();

	APrisonerDefenseGameModeBase* GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Player = GameMode->GetPlayer();

	MaxHealth = GameMode->GetNewMaxHealth();
	MovementSpeed = GameMode->GetNewMovementSpeed();

	Health = MaxHealth;
	HealthWidget = Cast<UPDHealthWidget>(Widget->GetWidget());

	HealthWidget->SetMeterFill(1);
}

// Called every frame
void APDPrisoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Defeated)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FVector targetPatrolPoint;
	FVector previousPatrolPoint;

	bool gotTargetPoint = TryGetTargetPoint(targetPatrolPoint);
	bool gotPreviousPoint = TryGetPreviousPoint(previousPatrolPoint);

	if (gotTargetPoint && gotPreviousPoint)
	{
		FVector lerpedPosition;

		targetPatrolPoint.Z = GetActorLocation().Z;
		previousPatrolPoint.Z = GetActorLocation().Z;

		float dstToPosition = FVector::Distance(GetActorLocation(), targetPatrolPoint);

		// Normalize the position offset for movement
		Velocity = targetPatrolPoint - previousPatrolPoint;
		Velocity.Normalize();
		Velocity *= MovementSpeed;

		FVector dir;
		float length;
		Velocity.ToDirectionAndLength(dir, length);

		if (dstToPosition > length * DeltaTime)
		{
			lerpedPosition = GetActorLocation() + (Velocity * DeltaTime);
		}
		else
		{
			TargetPatrolIndex++;
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

bool APDPrisoner::IsDefeated() const
{
	return Defeated;
}

bool APDPrisoner::TryGetTargetPoint(FVector& TargetPoint) const
{
	return Cage != nullptr && Cage->TryGetPatrolPoint(TargetPatrolIndex, TargetPoint);
}

bool APDPrisoner::TryGetPreviousPoint(FVector& PreviousPoint) const
{
	return Cage != nullptr && Cage->TryGetPatrolPoint(TargetPatrolIndex - 1, PreviousPoint);
}

