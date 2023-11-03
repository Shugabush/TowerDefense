// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTowerSlot.h"
#include "PDTower.h"

// Sets default values
APDTowerSlot::APDTowerSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void APDTowerSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDTowerSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

