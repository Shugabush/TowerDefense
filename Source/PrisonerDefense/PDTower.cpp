// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTower.h"

#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PrisonerDefenseGameModeBase.h"
#include "PDUpgradesWidget.h"
#include "MeshRenderData.h"

#include "Kismet/GameplayStatics.h"
#include <cassert>

// Sets default values
APDTower::APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh->AttachTo(RootComponent);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpgradesWidget"));
	Widget->AttachTo(RootComponent);

	VolumeTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("VolumeTrigger"));

	VolumeTrigger->OnComponentBeginOverlap.AddDynamic(this, &APDTower::OnVolumeTriggerBeginOverlap);
	VolumeTrigger->OnComponentEndOverlap.AddDynamic(this, &APDTower::OnVolumeTriggerEndOverlap);

	VolumeTrigger->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void APDTower::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<APrisonerDefenseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	UpgradesWidget = Cast<UPDUpgradesWidget>(Widget->GetWidget());

	assert(UpgradesWidget != nullptr && UpgradesWidget != NULL && "Failed to get upgrades widget. Maybe you need to respecify the type of widget to spawn");

	UpgradesWidget->ParentTower = this;

	MeshData = MeshRenderData(Mesh, "Color");

	Widget->SetVisibility(false);
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

}

void APDTower::OnTowerPlaced()
{

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
