// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CustomStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UCustomStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	virtual void OnComponentCreated() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FVector InitialRelativeLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FQuat InitialRelativeRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FVector InitialRelativeScale = FVector::OneVector;
	
};
