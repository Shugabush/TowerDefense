// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "PDBullet.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDBullet();

	// The object that the bullet is trying to damage or destroy
	class APDPrisoner* TargetVictim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector TargetVelocity;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	float MovementSpeed = 25;
};
