// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayer.h"

#include "PDShooter.h"
#include "PDTower.h"
#include "PDPowerGenerator.h"
#include "PDPrisonerCage.h"
#include "PDTurretSlot.h"
#include "PDTowerSlot.h"
#include "PDHUD.h"
#include "PDUserWidget.h"
#include "PDUpgradesWidget.h"
#include "PDPauseWidget.h"
#include "PrisonerDefenseGameModeBase.h"
#include "GameFramework/WorldSettings.h"

#include "Camera/CameraComponent.h"
#include "Materials/MaterialInterface.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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

UPDUserWidget* APDPlayer::GetWidget() const
{
	return Widget;
}

APDHUD* APDPlayer::GetHUD() const
{
	return HUD;
}

APrisonerDefenseGameModeBase* APDPlayer::GetGameMode()
{
	if (GameMode == nullptr)
	{
		GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	return GameMode;
}

APlayerController* APDPlayer::GetPlayerController()
{
	if (PlayerController == nullptr)
	{
		PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	}
	return PlayerController;
}

void APDPlayer::TogglePause()
{
	if (HUD != nullptr)
	{
		TSubclassOf<UPDPauseWidget> PauseWidgetClass = UPDPauseWidget::StaticClass();
		UPDPauseWidget* PauseWidget = nullptr;

		if (PlayerController->IsPaused())
		{
			// Resume
			HUD->DisableWidget(PauseWidgetClass);
			GetWorld()->GetWorldSettings()->SetTimeDilation(1);
			PlayerController->SetPause(false);
			PauseWidgetClass->GetDefaultObject<UPDPauseWidget>()->Resume();
		}
		else
		{
			// Pause
			HUD->EnableWidget<UPDPauseWidget>(UPDPauseWidget::StaticClass(), PauseWidget, false);
			GetWorld()->GetWorldSettings()->SetTimeDilation(0);
			PlayerController->SetPause(true);
			PauseWidgetClass->GetDefaultObject<UPDPauseWidget>()->Pause();
		}
	}
}

// Called when the game starts or when spawned
void APDPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	HUD = PlayerController->GetHUD<APDHUD>();

	Widget = HUD->GetWidget<UPDUserWidget>(UPDUserWidget::StaticClass());
	UpgradesWidget = HUD->GetWidget<UPDUpgradesWidget>(UPDUpgradesWidget::StaticClass());

	PlayerController->bShowMouseCursor = true;

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->OnVictory.AddDynamic(this, &APDPlayer::ClearTower);
	GameMode->OnLoss.AddDynamic(this, &APDPlayer::ClearTower);
}

void APDPlayer::OnMouseClicked()
{
	
}

void APDPlayer::OnMouseUp()
{
	if (ActiveTower == nullptr || SelectedTowerSlot == nullptr)
	{
		ChangeSelectedTower(PendingSelectedTower);
	}
	else
	{
		PlaceTower();
	}
}

// Called every frame
void APDPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Power = FMath::Clamp<float>(Power, 0, MaxPower);
	
	if (GameMode->GameIsRunning())
	{
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
		PlayerController->GetHitResultAtScreenPosition(MousePosition, ECollisionChannel::ECC_Camera, false, Result);
		ResultActor = Result.GetComponent()->GetAttachmentRootActor();

		if (ActiveTower != nullptr)
		{
			UpdateTower();
		}
		else
		{
			CheckForTowerSelection();
		}
	}
}

// Called to bind functionality to input
void APDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APDPlayer::OnMouseClicked);
	InputComponent->BindAction("MouseLeftClick", IE_Released, this, &APDPlayer::OnMouseUp);
	InputComponent->BindAction("PauseButton", IE_Pressed, this, &APDPlayer::TogglePause).bExecuteWhenPaused = true;
}

void APDPlayer::ClearTower()
{
	if (ActiveTower != nullptr)
	{
		ActiveTower->Destroy();
		ActiveTower = nullptr;
		SelectedTowerSlot = nullptr;
	}
}

void APDPlayer::PlaceTower()
{
	// Can't place the tower if it doesn't exist or the mouse isn't hovering over a tower slot
	if (ActiveTower == nullptr || SelectedTowerSlot == nullptr) { return; }

	Widget->PurchaseTargetWidget();

	ActiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Block);

	SelectedTowerSlot->Tower = ActiveTower;
	ActiveTower->ParentSlot = SelectedTowerSlot;
	ActiveTower->OnTowerPlaced();

	ChangeSelectedTower(ActiveTower);

	ActiveTower = nullptr;
	SelectedTowerSlot = nullptr;
}

void APDPlayer::UpdateTower()
{
	// Make sure ActiveObject doesn't block camera raycasts
	ActiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);

	if (ResultActor != nullptr)
	{
		ActiveTower->SetActorLocation(Result.ImpactPoint);

		SelectedTowerSlot = Cast<APDTowerSlot>(ResultActor);
	}
	else
	{
		SelectedTowerSlot = nullptr;
	}

	FLinearColor targetColor = SelectedTowerSlot != nullptr ? FLinearColor::White : FLinearColor::Red;

	ActiveTower->BlendMeshColors(targetColor);
}

void APDPlayer::CheckForTowerSelection()
{
	if (UpgradesWidget == nullptr) { return; }

	APDTower* LastPendingSelectedTower = PendingSelectedTower;
	if (ResultActor != nullptr)
	{
		PendingSelectedTower = Cast<APDTower>(ResultActor);

		if (PendingSelectedTower != LastPendingSelectedTower)
		{
			if (PendingSelectedTower != nullptr && SelectedTower != PendingSelectedTower)
			{
				PendingSelectedTower->OnMouseEnter();
			}
			if (LastPendingSelectedTower != nullptr && SelectedTower != LastPendingSelectedTower)
			{
				LastPendingSelectedTower->OnMouseExit();
			}
		}
	}

	UpgradesWidget->SetParentTower(SelectedTower);
}

void APDPlayer::ChangeSelectedTower(APDTower* NewSelectedTower)
{
	if (SelectedTower == NewSelectedTower)
	{
		return;
	}

	APDTower* PreviouslySelectedTower = SelectedTower;

	if (SelectedTower != nullptr)
	{
		SelectedTower->OnTowerDeselected(NewSelectedTower);
	}

	SelectedTower = NewSelectedTower;

	if (SelectedTower != nullptr)
	{
		SelectedTower->OnTowerSelected(PreviouslySelectedTower);
	}
}

void APDPlayer::UpdatePower(const float AdditionalPower)
{
	Power += AdditionalPower;

	Power = FMath::Clamp<float>(Power, 0, MaxPower);

	OnPowerChanged.Broadcast(Power);
}

float APDPlayer::GetPower() const
{
	return Power;
}

void APDPlayer::SpawnOrClearTower(TSubclassOf<APDTower> TowerBlueprint)
{
	if (ActiveTower != nullptr)
	{
		// Clear tower
		ClearTower();
		return;
	}

	// Only spawn a new tower if ActiveTower doesn't exist and the game isn't paused
	if (ActiveTower == nullptr && !PlayerController->IsPaused())
	{
		ActiveTower = GetWorld()->SpawnActor<APDTower>(TowerBlueprint);
		ActiveMesh = ActiveTower->GetMesh();
	}
}

