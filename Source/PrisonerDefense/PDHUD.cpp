// Fill out your copyright notice in the Description page of Project Settings.


#include "PDHUD.h"

#include "PDPlayer.h"

void APDHUD::EnableWidget(TSubclassOf<UUserWidget> WidgetClass, bool disableOthers)
{
	for (auto Widget : ActiveWidgets)
	{
		if (Widget != nullptr)
		{
			if (Widget->IsA(WidgetClass))
			{
				Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else if (disableOthers)
			{
				Widget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APDHUD::EnableWidgets(TArray<TSubclassOf<UUserWidget>> WidgetClasses, bool disableOthers)
{
	for (auto Widget : ActiveWidgets)
	{
		for (auto WidgetClass : WidgetClasses)
		{
			if (Widget != nullptr)
			{
				if (Widget->IsA(WidgetClass))
				{
					Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				else if (disableOthers)
				{
					Widget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

void APDHUD::DisableWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	for (auto Widget : ActiveWidgets)
	{
		if (Widget != nullptr && Widget->IsA(WidgetClass))
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APDHUD::DisableWidgets(TArray<TSubclassOf<UUserWidget>> WidgetClasses)
{
	for (auto Widget : ActiveWidgets)
	{
		for (auto WidgetClass : WidgetClasses)
		{
			if (Widget != nullptr && Widget->IsA(WidgetClass))
			{
				Widget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APDHUD::BeginPlay()
{
	// Create active widgets
	for (size_t i = 0; i < ActiveWidgetClasses.Num(); i++)
	{
		auto ActiveWidgetClass = ActiveWidgetClasses[i];
		if (ActiveWidgetClass != nullptr)
		{
			UUserWidget* activeWidget = CreateWidget<UUserWidget>(GetWorld(), ActiveWidgetClass);
			activeWidget->AddToViewport(InactiveWidgetClasses.Num() + 1);

			if (RootWidget == nullptr)
			{
				RootWidget = activeWidget;
			}

			ActiveWidgets.Add(activeWidget);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Null widget class at index " + FString::FromInt(i) + "!"));
		}
	}

	// Create inactive widgets
	for (size_t i = 0; i < InactiveWidgetClasses.Num(); i++)
	{
		auto InactiveWidgetClass = InactiveWidgetClasses[i];
		if (InactiveWidgetClass != nullptr)
		{
			UUserWidget* inactiveWidget = CreateWidget<UUserWidget>(GetWorld(), InactiveWidgetClass);
			inactiveWidget->AddToViewport(InactiveWidgetClasses.Num() - i);

			inactiveWidget->SetVisibility(ESlateVisibility::Hidden);

			ActiveWidgets.Add(inactiveWidget);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("No widget HUD class!"));
		}
	}
}
