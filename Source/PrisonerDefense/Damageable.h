// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PRISONERDEFENSE_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
public:
	UFUNCTION()
		virtual void Damage(int Damage);

	UFUNCTION()
		virtual void OnDefeat();
};