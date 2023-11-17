// Fill out your copyright notice in the Description page of Project Settings.


#include "PDGameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UPDGameOverWidget::SetResultsText(const bool victory)
{
	FText TargetText = FText::FromString("GAME OVER");
	if (victory)
	{
		TargetText = FText::FromString("VICTORY");
	}
	ResultsText->SetText(TargetText);
}

void UPDGameOverWidget::NativeConstruct()
{
	ReplayButton->OnClicked.AddDynamic(this, &UPDGameOverWidget::Replay);
	MainMenuButton->OnClicked.AddDynamic(this, &UPDGameOverWidget::MainMenu);
	QuitButton->OnClicked.AddDynamic(this, &UPDGameOverWidget::Quit);
}

void UPDGameOverWidget::Replay()
{
	UGameplayStatics::OpenLevel(GetWorld(), ReplayLevel);
}

void UPDGameOverWidget::MainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevel);
}

void UPDGameOverWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Type::Quit, false);
}
