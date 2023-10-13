// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayer.h"

#include "PDTurret.h"
#include "PDPrisonerCage.h"

#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
APDPlayer::APDPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeLocationAndRotation(FVector(0, 0, 1000), FQuat::MakeFromEuler(FVector(0, -90, 0)));

	RootComponent = Camera;

	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called when the game starts or when spawned
void APDPlayer::BeginPlay()
{
	Super::BeginPlay();

	Controller->bShowMouseCursor = true;
}

void APDPlayer::OnMouseClicked()
{
	FHitResult result;
	FVector2D mousePos;

	Controller->GetMousePosition(mousePos.X, mousePos.Y);

	Controller->GetHitResultAtScreenPosition(mousePos, ECollisionChannel::ECC_Camera, false, result);

	if (result.GetActor() != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, result.GetActor()->GetName());
	}
}

// Called every frame
void APDPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTurret != nullptr)
	{
		FHitResult result;
		FVector2D mousePos;

		Controller->GetMousePosition(mousePos.X, mousePos.Y);

		Controller->GetHitResultAtScreenPosition(mousePos, ECollisionChannel::ECC_Camera, false, result);

		ActiveTurret->SetActorLocation(result.ImpactPoint);

		AActor* resultActor = result.GetActor();

		if (resultActor != nullptr)
		{
			APDPrisonerCage* cage = Cast<APDPrisonerCage>(resultActor);

			// We can't place a turret on top of a prisoner cage
			ValidTurretPlacement = cage == nullptr;
		}
		else
		{
			ValidTurretPlacement = true;
		}
	}
	else
	{
		ValidTurretPlacement = false;
	}
}

// Called to bind functionality to input
void APDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APDPlayer::OnMouseClicked);
}

void APDPlayer::SpawnTurret()
{
	// Only spawn a new turret if ActiveTurret doesn't exist
	if (ActiveTurret == nullptr)
	{
		ActiveTurret = GetWorld()->SpawnActor<APDTurret>(TurretReference);
	}
}

