// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"

#include "PDPauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void Pause();

	UFUNCTION()
		void Resume();

	UFUNCTION()
		void Quit();
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* ResumeButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* QuitButton;

	UPROPERTY()
		class APDPlayer* Player;
};
