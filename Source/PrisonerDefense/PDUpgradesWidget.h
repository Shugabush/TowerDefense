// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Blueprint/UserWidget.h"
#include "PDUpgradesWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDUpgradesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetCurrentUpgradeCost() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool TryGetCurrentUpgradeCost(int& UpgradeCost) const;

	class APDTower* ParentTower;

	void InitializeUpgradeCosts(const TArray<int>& TargetCosts);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* UpgradeButton;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UTextBlock* UpgradeCostText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		TArray<int> UpgradeCosts;

	UPROPERTY()
		class UPDUserWidget* PlayerWidget;

	UFUNCTION()
		void OnUpgradeButtonClicked();

	int CurrentUpgradeIndex = 0;

	UFUNCTION()
		bool CanAffordNextUpgrade() const;
};
