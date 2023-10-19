// Fill out your copyright notice in the Description page of Project Settings.


#include "PDHUD.h"

#include "Blueprint/UserWidget.h"
#include "PDPlayer.h"

void APDHUD::BeginPlay()
{
	if (WidgetHUDClass != nullptr)
	{
		UUserWidget* rootWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);
		rootWidget->AddToViewport();

		RootWidget = rootWidget;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("No widget HUD class!"));
	}
}
