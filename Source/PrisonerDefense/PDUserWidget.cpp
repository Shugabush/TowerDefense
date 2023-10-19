// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUserWidget.h"
#include "PDPlayer.h"

void UPDUserWidget::NativeConstruct()
{
	GetOwningPlayerPawn<APDPlayer>();
}
