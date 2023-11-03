// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTower.h"

#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

#include "PrisonerDefenseGameModeBase.h"
#include "PDUpgradesWidget.h"
#include "MeshRenderData.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APDTower::APDTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VolumeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("VolumeTrigger"));

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

	MeshData = MeshRenderData(Mesh, "Color");

	Widget->SetVisibility(false);

	TArray<int> UpgradeCosts;
	for (FTowerUpgrade Upgrade : Upgrades)
	{
		UpgradeCosts.Add(Upgrade.GetPowerCost());
	}
	UpgradesWidget->InitializeUpgradeCosts(UpgradeCosts);
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

int FTowerUpgrade::GetPowerCost() const
{
	return 0;
}
