// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayer.h"

// Sets default values
APDPlayer::APDPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APDPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APDPlayer::OnMouseClicked()
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, TEXT("Mouse Pressed!"));
}

// Called every frame
void APDPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &APDPlayer::OnMouseClicked);
}

