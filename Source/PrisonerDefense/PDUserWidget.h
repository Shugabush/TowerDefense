// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Button.h"
#include "PDPlayer.h"

#include "Blueprint/UserWidget.h"
#include "PDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API UPDUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
		UButton* TurretButton;

	APDPlayer* OwningPlayer;
};
