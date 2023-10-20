// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Blueprint/UserWidget.h"
#include "PDPurchaseWidget.generated.h"

class UPDUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPurchaseSignature);

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
	UPDUserWidget* ParentWidget;

	UPROPERTY(BlueprintAssignable)
		FOnPurchaseSignature OnPurchase;

	int GetPrice() const;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		UTexture2D* ButtonIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		int Price = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* PurchaseButton;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UTextBlock* PriceText;

	UFUNCTION(BlueprintCallable)
		void TryPurchase();
};
