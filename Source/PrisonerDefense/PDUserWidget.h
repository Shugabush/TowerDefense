// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "PDPlayer.h"
#include "PDPurchaseWidget.h"

#include "Blueprint/UserWidget.h"
#include "PDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		int GetPower() const;

	UFUNCTION(BlueprintCallable)
		void UpdatePower(int additionalPower);

	UFUNCTION(BlueprintCallable)
		int GetTurretCost() const;

	UFUNCTION(BlueprintCallable)
		bool CanAffordTurret() const;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UPDPurchaseWidget* TurretPurchasable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UPDPurchaseWidget* PowerGeneratorPurchasable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UTextBlock* PowerText;

	APDPlayer* OwningPlayer;
	
	int Power = 250;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		int TurretCost = 100;
};
