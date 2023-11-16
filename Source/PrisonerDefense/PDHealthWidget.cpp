// Fill out your copyright notice in the Description page of Project Settings.


#include "PDHealthWidget.h"
#include "Components/ProgressBar.h"

void UPDHealthWidget::SetMeterFill(const float Amount)
{
	Meter->SetPercent(Amount);
	Meter->SetFillColorAndOpacity(FLinearColor::LerpUsingHSV(LowColor, HighColor, Amount));
}
