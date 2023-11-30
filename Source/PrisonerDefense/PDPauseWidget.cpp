// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPauseWidget.h"
#include "PDPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "CustomUtils.h"

void UPDPauseWidget::NativeConstruct()
{
	ResumeButton->OnClicked.AddDynamic(this, &UPDPauseWidget::Resume);
	QuitButton->OnClicked.AddDynamic(this, &UPDPauseWidget::Quit);

	Player = UCustomUtils::GetWorldPlayer(GetWorld(), 0);
}

void UPDPauseWidget::Pause()
{
	
}

void UPDPauseWidget::Resume()
{
	if (Player != nullptr)
	{
		Player->TogglePause();
	}
}

void UPDPauseWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Type::Quit, false);
}
