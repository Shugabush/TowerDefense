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

#include <cassert>

// Sets default values
APDTower::APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

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
	return FText();
}

// Called when the game starts or when spawned
void APDTower::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Player = GameMode->GetPlayer();

	MeshData = MeshRenderData(Mesh, "Color");
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
	CurrentUpgradeIndex++;
	UpgradesWidget->SetDescription(GetUpgradeDescription());
	RecieveUpgrade();
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
}

void APDTower::SetMeshColors(FLinearColor newColor)
{
	MeshData.SetColors(newColor);
}

void APDTower::ResetMeshColors()
{
	MeshData.ResetColors();
}

UStaticMeshComponent* APDTower::GetMesh()
{
	return Mesh;
}
