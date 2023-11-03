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
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetPower() const;

	UFUNCTION(BlueprintCallable)
		void UpdatePower(const int additionalPower);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetTurretCost() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetPowerGeneratorCost() const;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnTurretButtonClicked();

	UFUNCTION()
		void OnPowerGeneratorButtonClicked();

	UFUNCTION()
		void OnPlayButtonClicked();

	UFUNCTION()
		void OnRoundChanged(int NewRound);

	UFUNCTION()
		void OnRoundStarted();

	UFUNCTION()
		void OnRoundEnded();

private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UPDPurchaseWidget* TurretPurchasable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UPDPurchaseWidget* PowerGeneratorPurchasable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UTextBlock* PowerText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UTextBlock* RoundText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* PlayButton;

	UPROPERTY()
		APDPlayer* OwningPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		FString PowerTextPrefix = "Power: ";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		FString RoundTextPrefix = "Round: ";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		int Power = 500;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		int TurretCost = 100;
};
