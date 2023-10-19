// Fill out your copyright notice in the Description page of Project Settings.


#include "PDHUD.h"

#include "Blueprint/UserWidget.h"
#include "PDPlayer.h"

void APDHUD::BeginPlay()
{
	UUserWidget* rootWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);
	rootWidget->AddToViewport();

	RootWidget = rootWidget;
}
