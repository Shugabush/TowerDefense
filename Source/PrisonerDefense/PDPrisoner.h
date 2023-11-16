// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"

#include "GameFramework/Pawn.h"
#include "Damageable.h"
#include "PDPrisoner.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPrisoner : public AActor, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APDPrisoner();

	virtual void Damage(int Damage) override;
	virtual void OnDefeat() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int Health;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		int MaxHealth = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FVector GetGroundVelocity() const;

	UPROPERTY()
		// The prisoner cage will assign this pointer
		class APDPrisonerCage* Cage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		class UWidgetComponent* Widget;

	UPROPERTY()
		// Widget property as health widget
		class UPDHealthWidget* HealthWidget;

	bool IsDefeated() const;
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		// Power that the player earns by damaging this prisoner
		int PowerReward = 10;

	UPROPERTY()
		class APDPlayer* Player;

	FVector Velocity;
	FQuat TargetRotation;
	
	bool Defeated = false;

	int TargetIndex;
	bool TryGetTargetPoint(FVector& TargetPoint) const;
	bool TryGetPreviousPoint(FVector& PreviousPoint) const;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float MovementSpeed = 100;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float RotationSpeed = 10;
};
