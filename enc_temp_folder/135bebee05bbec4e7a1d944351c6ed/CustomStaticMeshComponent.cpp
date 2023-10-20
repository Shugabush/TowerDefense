// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStaticMeshComponent.h"

void UCustomStaticMeshComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	FVector FinalLocation = InitialRelativeRotation.RotateVector(InitialRelativeLocation) * InitialRelativeScale;
	SetRelativeLocationAndRotation(FinalLocation, InitialRelativeRotation);
	SetRelativeScale3D(InitialRelativeScale);
}
