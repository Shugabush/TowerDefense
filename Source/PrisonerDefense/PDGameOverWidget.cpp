// Fill out your copyright notice in the Description page of Project Settings.


#include "PDGameOverWidget.h"

void UPDGameOverWidget::NativeConstruct()
{
	ReplayButton->OnClicked.AddDynamic(this, &UPDGameOverWidget::Replay);
	MainMenuButton->OnClicked.AddDynamic(this, &UPDGameOverWidget::MainMenu);
	QuitButton->OnClicked.AddDynamic(this, &UPDGameOverWidget::Quit);
}

void UPDGameOverWidget::Replay()
{

}

void UPDGameOverWidget::MainMenu()
{

}

void UPDGameOverWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Type::Quit, false);
}
