// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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

	void SetParentTower(class APDTower* NewParentTower);

	void InitializeUpgradeCosts(const TArray<int>& TargetCosts);

	void SetDescription(FText Description);

	void SwipeIn();
	void SwipeOut();

	void UpdateDisplay();

	void SetTowerIcon(UTexture2D* Texture);

protected:
	virtual void NativeConstruct() override;

	void AssignAnimations();

	TMap<FString, UWidgetAnimation*> Animations;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UButton* UpgradeButton;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* UpgradeCostText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* UpgradeDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		// Icon that will display the selected tower
		class UImage* TowerIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		TArray<int> UpgradeCosts;

	UPROPERTY()
		class APDTower* ParentTower;

	UPROPERTY()
		class APDPlayer* Player;

	UPROPERTY()
		class UPDUserWidget* PlayerWidget;

	UFUNCTION()
		void OnUpgradeButtonClicked();

	int CurrentUpgradeIndex = 0;

	UFUNCTION()
		bool CanAffordNextUpgrade() const;
};
