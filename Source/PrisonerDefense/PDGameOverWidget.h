// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Blueprint/UserWidget.h"
#include "PDGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetResultsText(const bool victory);
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FName ReplayLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FName MainMenuLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* ResultsText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		class UButton* ReplayButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* MainMenuButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* QuitButton;

	UFUNCTION()
		void Replay();

	UFUNCTION()
		void MainMenu();

	UFUNCTION()
		void Quit();
};
