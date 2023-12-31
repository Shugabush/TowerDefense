// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "PDPurchaseWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPurchaseSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedSignature);

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDPurchaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;

public:
	UPROPERTY()
		class UPDUserWidget* ParentWidget;

	UPROPERTY(BlueprintAssignable)
		FOnButtonClickedSignature OnButtonClicked;

	UPROPERTY(BlueprintAssignable)
		FOnPurchaseSignature OnPurchase;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetPrice() const;

	UFUNCTION(BlueprintCallable)
		void SetButtonEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
		void OnPowerUpdated(const float PowerValue);

	void OnPurchased();

private:
	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDTower> TowerBlueprint;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		UTexture2D* ButtonIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		float Price = 100.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		float PriceIncreasePerPurchase = 25.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UButton* PurchaseButton;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* PriceText;

	UFUNCTION(BlueprintCallable)
		void ButtonClicked();
};
