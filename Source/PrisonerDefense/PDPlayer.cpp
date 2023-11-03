// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayer.h"

#include "PDTurret.h"
#include "PDTower.h"
#include "PDPowerGenerator.h"
#include "PDPrisonerCage.h"
#include "PDTurretSlot.h"
#include "PDTowerSlot.h"
#include "PDHUD.h"
#include "PDUserWidget.h"
#include "PDPauseWidget.h"
#include "PrisonerDefenseGameModeBase.h"
#include "GameFramework/WorldSettings.h"

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

UPDUserWidget* APDPlayer::GetWidget() const
{
	return Widget;
}

// Called when the game starts or when spawned
void APDPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	HUD = PlayerController->GetHUD<APDHUD>();
	Widget = HUD->GetWidget<UPDUserWidget>(UPDUserWidget::StaticClass());

	PlayerController->bShowMouseCursor = true;

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void APDPlayer::OnMouseClicked()
{
	PlaceTower();
}

void APDPlayer::OnPauseButtonPressed()
{
	if (HUD != nullptr)
	{
		TSubclassOf<UUserWidget> PauseWidget = UPDPauseWidget::StaticClass();

		if (PlayerController->IsPaused())
		{
			// Resume
			HUD->DisableWidget(PauseWidget);
			GetWorld()->GetWorldSettings()->SetTimeDilation(1);
			PlayerController->SetPause(false);
			PauseWidget->GetDefaultObject<UPDPauseWidget>()->Resume();
		}
		else
		{
			// Pause
			HUD->EnableWidget(PauseWidget);
			GetWorld()->GetWorldSettings()->SetTimeDilation(0);
			PlayerController->SetPause(true);
			PauseWidget->GetDefaultObject<UPDPauseWidget>()->Pause();
		}
	}
}

// Called every frame
void APDPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTower != nullptr)
	{
		UpdateTower();
	}
}

// Called to bind functionality to input
void APDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APDPlayer::OnMouseClicked);
	InputComponent->BindAction("PauseButton", IE_Pressed, this, &APDPlayer::OnPauseButtonPressed).bExecuteWhenPaused = true;
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

void APDPlayer::SpawnTurret()
{
	if (HasPowerGenerator())
	{
		// Clear power generator
		ClearTower();
	}

	// Only spawn a new turret if ActiveTurret doesn't exist and the game isn't paused
	if (ActiveTower == nullptr && !PlayerController->IsPaused())
	{
		ActiveTower = GetWorld()->SpawnActor<APDTurret>(TurretReference);
		ActiveMesh = ActiveTower->GetMesh();
	}
}

void APDPlayer::PlaceTower()
{
	// Can't place the tower if it doesn't exist
	if (ActiveTower == nullptr) { return; }

	ActiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Block);

	SelectedTowerSlot->Tower = ActiveTower;
	ActiveTower->ParentSlot = SelectedTowerSlot;
	ActiveTower->OnTowerPlaced();

	ActiveTower = nullptr;
	SelectedTowerSlot = nullptr;

	Widget->UpdatePower(-Widget->GetTurretCost());
}

void APDPlayer::UpdateTower()
{
	// Make sure ActiveObject doesn't block camera raycasts
	ActiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);

	FHitResult result;
	FVector2D mousePos;

	PlayerController->GetMousePosition(mousePos.X, mousePos.Y);

	PlayerController->GetHitResultAtScreenPosition(mousePos, ECollisionChannel::ECC_Camera, false, result);

	AActor* resultActor = result.GetActor();

	if (resultActor != nullptr)
	{
		ActiveTower->SetActorLocation(result.ImpactPoint);

		SelectedTowerSlot = Cast<APDTowerSlot>(resultActor);
	}
	else
	{
		SelectedTowerSlot = nullptr;
	}

	FLinearColor targetColor = SelectedTowerSlot != nullptr ? FLinearColor::White : FLinearColor::Red;

	ActiveTower->BlendMeshColors(targetColor);
}

void APDPlayer::SpawnPowerGenerator()
{
	if (HasTurret())
	{
		// Clear existing turret
		ClearTower();
	}

	// Only spawn a new power generator if ActivePowerGenerator doesn't exist and the game isn't paused
	if (ActiveTower == nullptr && !PlayerController->IsPaused())
	{
		ActiveTower = GetWorld()->SpawnActor<APDPowerGenerator>(PowerGeneratorReference);
		ActiveTower->Player = this;
		ActiveMesh = ActiveTower->GetMesh();
	}
}

void APDPlayer::OnTurretButtonClicked()
{
	if (HasTurret())
	{
		ClearTower();
	}
	else
	{
		SpawnTurret();
	}
}

void APDPlayer::OnPowerGeneratorButtonClicked()
{
	if (HasPowerGenerator())
	{
		ClearTower();
	}
	else
	{
		SpawnPowerGenerator();
	}
}

APrisonerDefenseGameModeBase* APDPlayer::GetGameMode() const
{
	return GameMode;
}

bool APDPlayer::HasTurret() const
{
	if (ActiveTower == nullptr) { return false; }

	APDTurret* ActiveTurret = Cast<APDTurret>(ActiveTower);
	return ActiveTurret != nullptr;
}

bool APDPlayer::HasPowerGenerator() const
{
	if (ActiveTower == nullptr) { return false; }

	APDPowerGenerator* ActivePowerGenerator = Cast<APDPowerGenerator>(ActiveTower);
	return ActivePowerGenerator != nullptr;
}

