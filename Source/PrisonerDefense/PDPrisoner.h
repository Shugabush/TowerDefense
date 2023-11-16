// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"

#include "GameFramework/Pawn.h"
#include "PDPrisoner.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPrisoner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APDPrisoner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
		FVector GetGroundVelocity() const;

	// The prisoner cage will assign this pointer
	UPROPERTY()
		class APDPrisonerCage* Cage;

	bool IsDefeated() const;
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		// Power that the player earns by defeating this prisoner
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
