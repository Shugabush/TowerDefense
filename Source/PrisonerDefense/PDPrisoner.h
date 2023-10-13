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

	UFUNCTION(BlueprintCallable)
		FVector GetGroundVelocity() const;

	// The prisoner cage will assign this pointer
	class APDPrisonerCage* Cage;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* Mesh;

	FVector Velocity;
	FQuat TargetRotation;

	int TargetIndex;
	bool TryGetTargetPoint(FVector& TargetPoint) const;
	bool TryGetPreviousPoint(FVector& PreviousPoint) const;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float MovementSpeed = 1;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		float RotationSpeed = 10;
};
