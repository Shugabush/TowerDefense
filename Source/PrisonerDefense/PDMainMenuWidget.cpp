// Fill out your copyright notice in the Description page of Project Settings.


#include "PDMainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UPDMainMenuWidget::NativeConstruct()
{
	if (PlayButton != nullptr)
	{
		PlayButton->OnClicked.AddDynamic(this, &UPDMainMenuWidget::Play);
	}

	if (QuitButton != nullptr)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPDMainMenuWidget::Quit);
	}
}

void UPDMainMenuWidget::Play()
{
	UGameplayStatics::OpenLevel(this, TEXT("GameMap"));
}

void UPDMainMenuWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Type::Quit, false);
}
