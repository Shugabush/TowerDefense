// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTower.h"

#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PrisonerDefenseGameModeBase.h"
#include "PDPlayer.h"
#include "PDHUD.h"
#include "PDUpgradesWidget.h"
#include "MeshRenderData.h"
#include "CustomUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APDTower::APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Type::Movable);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(RootComponent);

	VolumeTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("VolumeTrigger"));

	VolumeTrigger->OnComponentBeginOverlap.AddDynamic(this, &APDTower::OnVolumeTriggerBeginOverlap);
	VolumeTrigger->OnComponentEndOverlap.AddDynamic(this, &APDTower::OnVolumeTriggerEndOverlap);

	VolumeTrigger->AttachTo(RootComponent);

	SelectionRadius = CreateDefaultSubobject<USphereComponent>("SelectionRadius");
	SelectionRadius->AttachTo(RootComponent);
	SelectionRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	RangeIndicator = CreateDefaultSubobject<UStaticMeshComponent>("RangeIndicator");
	RangeIndicator->AttachTo(RootComponent);
	RangeIndicator->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void APDTower::OnMouseEnter()
{
	RangeIndicator->SetVisibility(true);
	RecieveOnMouseEnter();
}

void APDTower::OnMouseExit()
{
	RangeIndicator->SetVisibility(false);
	RecieveOnMouseExit();
}

void APDTower::OnTowerSelected(APDTower* PreviouslySelectedTower)
{
	RangeIndicator->SetVisibility(true);
	Player->GetHUD()->EnableWidget<UPDUpgradesWidget>(UPDUpgradesWidget::StaticClass(), UpgradesWidget);
	UpgradesWidget->SetParentTower(this);
	UpgradesWidget->InitializeUpgradeCosts(UpgradeCosts);
	UpgradesWidget->UpdateDisplay();
	if (PreviouslySelectedTower == nullptr)
	{
		UpgradesWidget->SwipeIn();
	}
	RecieveOnTowerSelected(PreviouslySelectedTower);
}

void APDTower::OnTowerDeselected(APDTower* NewSelectedTower)
{
	RangeIndicator->SetVisibility(false);
	if (NewSelectedTower == nullptr)
	{
		UpgradesWidget->SwipeOut();
	}
	RecieveOnTowerDeselected(NewSelectedTower);
}

FText APDTower::GetUpgradeDescription() const
{
	return FText::FromString("Upgrade\n");
}

FText APDTower::GetCurrentDescription() const
{
	return FText::FromString("Current\n");
}

UTexture2D* APDTower::GetIcon() const
{
	return Icon;
}

// Called when the game starts or when spawned
void APDTower::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Player = GameMode->GetPlayer();

	MeshData = MeshRenderData(Mesh, "Color");
	for (auto ExtraMesh : ExtraMeshes)
	{
		if (ExtraMesh != nullptr)
		{
			ExtraMeshData.Add(MeshRenderData(ExtraMesh, "Color"));
		}
	}

	VolumeTriggerRadius = VolumeTrigger->GetUnscaledSphereRadius();
	RangeIndicatorScale = RangeIndicator->GetRelativeScale3D();

	GameMode->OnRoundStarted.AddDynamic(this, &APDTower::OnRoundStarted);
	GameMode->OnRoundEnded.AddDynamic(this, &APDTower::OnRoundEnded);
}

void APDTower::OnRoundStarted()
{

}

void APDTower::OnRoundEnded()
{
	if (UpgradesWidget != nullptr)
	{
		UpgradesWidget->UpdateDisplay();
	}
}

void APDTower::OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void APDTower::OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

// Called every frame
void APDTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APDTower::Upgrade()
{
	RecievePreUpgrade(CurrentUpgradeIndex);
	CurrentUpgradeIndex++;
	if (UpgradesWidget != nullptr)
	{
		UpgradesWidget->UpdateDisplay();
	}
	RecievePostUpgrade(CurrentUpgradeIndex);
}

void APDTower::OnTowerPlaced()
{
	// Make the selection radius detectable from the camera
	SelectionRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
}

int APDTower::GetCurrentUpgradeIndex() const
{
	return CurrentUpgradeIndex;
}

TArray<int> APDTower::GetUpgradeCosts() const
{
	return TArray<int>(UpgradeCosts);
}

void APDTower::BlendMeshColors(FLinearColor newColor)
{
	MeshData.BlendColors(newColor);
	for (auto Data : ExtraMeshData)
	{
		Data.BlendColors(newColor);
	}
}

void APDTower::SetMeshColors(FLinearColor newColor)
{
	MeshData.SetColors(newColor);
	for (auto Data : ExtraMeshData)
	{
		Data.SetColors (newColor);
	}
}

void APDTower::ResetMeshColors()
{
	MeshData.ResetColors();
	for (auto Data : ExtraMeshData)
	{
		Data.ResetColors();
	}
}

UStaticMeshComponent* APDTower::GetMesh()
{
	return Mesh;
}
