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
		float GetPlayerPower() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetTurretCost() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetPowerGeneratorCost() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class APDPlayer* GetPlayer() const;

	UFUNCTION(BlueprintCallable)
		void PurchaseTargetWidget();

	class APrisonerDefenseGameModeBase* GetGameMode() const;

	UPROPERTY()
		class UPDPurchaseWidget* TargetPurchasable;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnPowerUpdated(const float PowerValue);

	UFUNCTION()
		void OnPlayButtonClicked();

	UFUNCTION()
		void OnRoundChanged(const int NewRound);

	UFUNCTION()
		void OnRoundStarted();

	UFUNCTION()
		void OnRoundEnded();

private:
	UPROPERTY()
		TArray<class UPDPurchaseWidget*> ChildPurchaseWidgets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UPanelWidget* Panel;

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
};
