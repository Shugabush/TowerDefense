// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayer.h"

#include "PDTurret.h"
#include "PDPowerGenerator.h"
#include "PDPrisonerCage.h"
#include "PDTurretSlot.h"
#include "PDPowerGeneratorSlot.h"
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
	if (SelectedTurretSlot != nullptr)
	{
		PlaceTurret();
	}
	else if (SelectedPowerGeneratorSlot != nullptr)
	{
		PlacePowerGenerator();
	}
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

	if (HasTurret())
	{
		UpdateTurret();
	}
	if (HasPowerGenerator())
	{
		UpdatePowerGenerator();
	}
}

// Called to bind functionality to input
void APDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APDPlayer::OnMouseClicked);
	InputComponent->BindAction("PauseButton", IE_Pressed, this, &APDPlayer::OnPauseButtonPressed).bExecuteWhenPaused = true;
}

void APDPlayer::ClearTurret()
{
	if (ActiveObject != nullptr)
	{
		ActiveObject->Destroy();
		ActiveObject = nullptr;
		ActiveTurret = nullptr;
		SelectedTurretSlot = nullptr;
	}
}

void APDPlayer::SpawnTurret()
{
	if (HasPowerGenerator())
	{
		// Clear power generator
		ClearPowerGenerator();
	}

	// Only spawn a new turret if ActiveTurret doesn't exist and the game isn't paused
	if (ActiveObject == nullptr && !PlayerController->IsPaused())
	{
		ActiveTurret = GetWorld()->SpawnActor<APDTurret>(TurretReference);
		ActiveObject = ActiveTurret;
		ActiveMesh = ActiveTurret->GetMesh();
	}
}

void APDPlayer::PlaceTurret()
{
	// Can't place the turret if it doesn't exist
	if (!HasTurret()) { return; }

	ActiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Block);

	SelectedTurretSlot->Turret = ActiveTurret;
	ActiveTurret->ParentSlot = SelectedTurretSlot;

	ActiveObject = nullptr;
	ActiveTurret = nullptr;

	Widget->UpdatePower(-Widget->GetTurretCost());
}

void APDPlayer::UpdateTurret()
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
		ActiveObject->SetActorLocation(result.ImpactPoint);

		SelectedTurretSlot = Cast<APDTurretSlot>(resultActor);
	}
	else
	{
		SelectedTurretSlot = nullptr;
	}

	FLinearColor targetColor = SelectedTurretSlot != nullptr ? FLinearColor::White : FLinearColor::Red;

	ActiveTurret->BlendMeshColors(targetColor);
}

void APDPlayer::ClearPowerGenerator()
{
	if (HasPowerGenerator())
	{
		ActiveObject->Destroy();
		ActiveObject = nullptr;
		ActivePowerGenerator = nullptr;
		SelectedPowerGeneratorSlot = nullptr;
	}
}

void APDPlayer::SpawnPowerGenerator()
{
	if (HasTurret())
	{
		// Clear existing turret
		ClearTurret();
	}

	// Only spawn a new power generator if ActivePowerGenerator doesn't exist and the game isn't paused
	if (ActiveObject == nullptr && !PlayerController->IsPaused())
	{
		ActivePowerGenerator = GetWorld()->SpawnActor<APDPowerGenerator>(PowerGeneratorReference);
		ActivePowerGenerator->Player = this;
		ActiveObject = ActivePowerGenerator;
		ActiveMesh = ActivePowerGenerator->GetMesh();
	}
}

void APDPlayer::PlacePowerGenerator()
{
	// Can't place the power generator if it doesn't exist
	if (!HasPowerGenerator()) 
	{ 
		return; 
	}

	ActiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Block);

	ActivePowerGenerator->ParentSlot = SelectedPowerGeneratorSlot;

	ActiveObject = nullptr;
	ActivePowerGenerator = nullptr;

	Widget->UpdatePower(-Widget->GetPowerGeneratorCost());
}

void APDPlayer::UpdatePowerGenerator()
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
		ActiveObject->SetActorLocation(result.ImpactPoint);

		SelectedPowerGeneratorSlot = Cast<APDPowerGeneratorSlot>(resultActor);
	}
	else
	{
		SelectedPowerGeneratorSlot = nullptr;
	}

	FLinearColor targetColor = SelectedPowerGeneratorSlot != nullptr ? FLinearColor::White : FLinearColor::Red;

	ActivePowerGenerator->BlendMeshColors(targetColor);
}

void APDPlayer::OnTurretButtonClicked()
{
	if (HasTurret())
	{
		ClearTurret();
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
		ClearPowerGenerator();
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

bool APDPlayer::HasTurret()
{
	if (ActiveObject == nullptr) { return false; }

	if (ActiveTurret == nullptr)
	{
		ActiveTurret = Cast<APDTurret>(ActiveObject);
	}

	return ActiveTurret != nullptr;
}

bool APDPlayer::HasPowerGenerator()
{
	if (ActiveObject == nullptr) { return false; }

	if (ActivePowerGenerator == nullptr)
	{
		ActivePowerGenerator = Cast<APDPowerGenerator>(ActiveObject);
	}

	return ActivePowerGenerator != nullptr;
}

