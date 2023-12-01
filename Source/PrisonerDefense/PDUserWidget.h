// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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

	UFUNCTION(BlueprintCallable)
		void PurchaseTurret();

	UFUNCTION(BlueprintCallable)
		void PurchasePowerGenerator();

	UFUNCTION(BlueprintCallable)
		class APDPlayer* GetPlayer() const;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	class APrisonerDefenseGameModeBase* GetGameMode() const;
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UPDPurchaseWidget* TurretPurchasable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UPDPurchaseWidget* PowerGeneratorPurchasable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* PowerText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* RoundText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UButton* PlayButton;

	UPROPERTY(BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = true))
		class APrisonerDefenseGameModeBase* GameMode;

	UPROPERTY()
		class APDPlayer* OwningPlayer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		FString PowerTextPrefix = "Power: ";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		FString RoundTextPrefix = "Round: ";
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		int Power = 500;
};
