// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPowerGenerator.h"

#include "Components/StaticMeshComponent.h"
#include "CustomStaticMeshComponent.h"

// Sets default values
APDPowerGenerator::APDPowerGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UCustomStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh->AttachTo(RootComponent);
}

UStaticMeshComponent* APDPowerGenerator::GetMesh() const
{
	return Mesh;
}

// Called when the game starts or when spawned
void APDPowerGenerator::BeginPlay()
{
	Super::BeginPlay();

	MeshData = MeshRenderData(Mesh, "Color");
}

// Called every frame
void APDPowerGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If parent slot is not nullptr, then we've actually placed this turret down
	if (ParentSlot != nullptr)
	{
		
	}
}

void APDPowerGenerator::BlendMeshColors(FLinearColor newColor)
{
	MeshData.BlendColors(newColor);
}

void APDPowerGenerator::SetMeshColors(FLinearColor newColor)
{
	MeshData.SetColors(newColor);
}

void APDPowerGenerator::ResetMeshColors()
{
	MeshData.ResetColors();
}

