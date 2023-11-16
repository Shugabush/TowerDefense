// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMeterFill(const float Amount);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget))
		class UProgressBar* Meter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		// Color to lerp towards when the meter is at a higher percent
		FLinearColor HighColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		// Color to lerp towards when the meter is at a lower percent
		FLinearColor LowColor = FLinearColor::Red;
};
