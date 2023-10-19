// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPauseWidget.h"
#include "Kismet/GameplayStatics.h"

void UPDPauseWidget::NativeConstruct()
{
	ResumeButton->OnClicked.AddDynamic(this, &UPDPauseWidget::Resume);
	QuitButton->OnClicked.AddDynamic(this, &UPDPauseWidget::Quit);
}

void UPDPauseWidget::Pause()
{
	
}

void UPDPauseWidget::Resume()
{

}

void UPDPauseWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Type::Quit, false);
}
