// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayer.h"

#include "PDTurret.h"
#include "PDPrisonerCage.h"
#include "PDTurretSlot.h"
#include "PDHUD.h"
#include "PDPauseWidget.h"

#include "Camera/CameraComponent.h"
#include "Materials/MaterialInterface.h"
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

	HUD = Cast<APDHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	Controller->bShowMouseCursor = true;
}

void APDPlayer::OnMouseClicked()
{
	if (SelectedSlot != nullptr)
	{
		PlaceTurret();
	}
}

void APDPlayer::OnPauseButtonPressed()
{
	if (HUD != nullptr)
	{
		TSubclassOf<UUserWidget> PauseWidget = UPDPauseWidget::StaticClass();
		HUD->SetWidgetActive(PauseWidget);
	}
}

// Called every frame
void APDPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTurret != nullptr)
	{
		// Make sure ActiveTurret doesn't block camera raycasts
		ActiveTurret->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);

		FHitResult result;
		FVector2D mousePos;

		Controller->GetMousePosition(mousePos.X, mousePos.Y);

		Controller->GetHitResultAtScreenPosition(mousePos, ECollisionChannel::ECC_Camera, false, result);

		AActor* resultActor = result.GetActor();

		if (resultActor != nullptr)
		{
			ActiveTurret->SetActorLocation(result.ImpactPoint);

			SelectedSlot = Cast<APDTurretSlot>(resultActor);
		}
		else
		{
			SelectedSlot = nullptr;
		}

		FLinearColor targetColor = SelectedSlot != nullptr ? FLinearColor::White : FLinearColor::Red;

		ActiveTurret->BlendMeshColors(targetColor);
	}
	else
	{
		SelectedSlot = nullptr;
	}
}

// Called to bind functionality to input
void APDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APDPlayer::OnMouseClicked);
	InputComponent->BindAction("PauseButton", IE_Pressed, this, &APDPlayer::OnPauseButtonPressed);
}

void APDPlayer::ClearTurret()
{
	ActiveTurret->Destroy();

	ActiveTurret = nullptr;
}

void APDPlayer::SpawnTurret()
{
	// Only spawn a new turret if ActiveTurret doesn't exist
	if (ActiveTurret == nullptr)
	{
		ActiveTurret = GetWorld()->SpawnActor<APDTurret>(TurretReference);
	}
}

void APDPlayer::PlaceTurret()
{
	// Can't place the turret if it doesn't exist
	if (ActiveTurret == nullptr) { return; }

	ActiveTurret->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Block);

	SelectedSlot->Turret = ActiveTurret;
	ActiveTurret->ParentSlot = SelectedSlot;

	ActiveTurret = nullptr;
}

void APDPlayer::OnTurretButtonClicked()
{
	if (ActiveTurret == nullptr)
	{
		SpawnTurret();
	}
	else
	{
		ClearTurret();
	}
}

bool APDPlayer::HasTurret()
{
	return ActiveTurret != nullptr;
}

bool APDPlayer::GetIsPaused() const
{
	return IsPaused;
}

