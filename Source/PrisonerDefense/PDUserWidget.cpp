// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUserWidget.h"
#include "PDPlayer.h"

void UPDUserWidget::NativeConstruct()
{
	OwningPlayer = GetOwningPlayerPawn<APDPlayer>();

	if (TurretButton != nullptr)
	{
		TurretButton->OnClicked.AddDynamic(OwningPlayer, &APDPlayer::OnTurretButtonClicked);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Turret button hasn't been assigned!"));
	}
}
