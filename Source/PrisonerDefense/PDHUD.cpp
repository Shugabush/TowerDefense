// Fill out your copyright notice in the Description page of Project Settings.


#include "PDHUD.h"

#include "Blueprint/UserWidget.h"
#include "PDPlayer.h"

void APDHUD::EnableWidget(TSubclassOf<UUserWidget> WidgetClass, bool disableOthers)
{
	for (auto Widget : ActiveWidgets)
	{
		if (Widget != nullptr)
		{
			if (Widget->IsA(WidgetClass))
			{
				Widget->SetVisibility(ESlateVisibility::Visible);
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
					Widget->SetVisibility(ESlateVisibility::Visible);
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
	for (size_t i = 0; i < WidgetHUDClasses.Num(); i++)
	{
		auto WidgetHUDClass = WidgetHUDClasses[i];
		if (WidgetHUDClass != nullptr)
		{
			UUserWidget* otherWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);
			otherWidget->AddToViewport(WidgetHUDClasses.Num() - i);

			otherWidget->SetVisibility(ESlateVisibility::Hidden);

			ActiveWidgets.Add(otherWidget);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("No widget HUD class!"));
		}
	}
	if (WidgetRootHUDClass != nullptr)
	{
		UUserWidget* rootWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetRootHUDClass);
		rootWidget->AddToViewport(WidgetHUDClasses.Num() + 1);

		RootWidget = rootWidget;

		ActiveWidgets.Add(rootWidget);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("No widget HUD class!"));
	}
}
