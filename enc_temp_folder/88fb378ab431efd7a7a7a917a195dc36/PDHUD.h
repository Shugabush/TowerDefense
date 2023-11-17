// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Blueprint/UserWidget.h"
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
	void EnableWidget(TSubclassOf<UUserWidget> WidgetClass, bool disableOthers = false);

	template<typename T>
	void EnableWidget(TSubclassOf<T> WidgetClass, T*& Widget, bool disableOthers = false)
	{
		for (auto ActiveWidget : ActiveWidgets)
		{
			if (ActiveWidget != nullptr)
			{
				if (ActiveWidget->IsA(WidgetClass))
				{
					ActiveWidget->SetVisibility(ESlateVisibility::Visible);
					Widget = Cast<T>(ActiveWidget);
				}
				else if (disableOthers)
				{
					ActiveWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}

	void EnableWidgets(TArray<TSubclassOf<UUserWidget>> WidgetClasses, bool disableOthers = false);

	void DisableWidget(TSubclassOf<UUserWidget> WidgetClass);
	void DisableWidgets(TArray<TSubclassOf<UUserWidget>> WidgetClasses);

	template<typename T>
	T* GetWidget(TSubclassOf<T> WidgetClass) const
	{
		for (auto Widget : ActiveWidgets)
		{
			if (Widget->IsA(WidgetClass))
			{
				return Cast<T>(Widget);
			}
		}

		return nullptr;
	}

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true, ToolTip = "Active widgets to spawn. The first one will be the root widget."))
		TArray<TSubclassOf<UUserWidget>> ActiveWidgetClasses;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true, ToolTip = "Inactive widgets to spawn."))
		TArray<TSubclassOf<UUserWidget>> InactiveWidgetClasses;

	TArray<UUserWidget*> ActiveWidgets;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UUserWidget* RootWidget;
};
