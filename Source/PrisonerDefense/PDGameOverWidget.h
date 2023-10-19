// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"

#include "Blueprint/UserWidget.h"
#include "PDGameOverWidget.generated.h"

#include "Kismet/KismetSystemLibrary.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
		
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* ReplayButton;

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
