// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/HUD.h"

#include "PDHUD.generated.h"

/**
 * 
 */
UCLASS()
class PRISONERDEFENSE_API APDHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetWidgetActive(TSubclassOf<UUserWidget> WidgetClass);
	void SetWidgetsActive(TArray<TSubclassOf<UUserWidget>> WidgetClasses);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> WidgetRootHUDClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<UUserWidget>> WidgetHUDClasses;

	TArray<UUserWidget*> ActiveWidgets;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UUserWidget* RootWidget;
};
