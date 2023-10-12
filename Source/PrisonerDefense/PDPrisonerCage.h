// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDPrisonerCage.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPrisonerCage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDPrisonerCage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
