// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDPlayer.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APDPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnMouseClicked();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};