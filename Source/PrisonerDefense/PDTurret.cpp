// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurret.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
APDTurret::APDTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh->AttachTo(RootComponent);
}

UStaticMeshComponent* APDTurret::GetMesh()
{
	return Mesh;
}

// Called when the game starts or when spawned
void APDTurret::BeginPlay()
{
	Super::BeginPlay();
	
	MeshData = MeshRenderData(Mesh, "Color");
}

// Called every frame
void APDTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APDTurret::BlendMeshColors(FLinearColor newColor)
{
	MeshData.BlendColors(newColor);
}

void APDTurret::SetMeshColors(FLinearColor newColor)
{
	MeshData.SetColors(newColor);
}

void APDTurret::ResetMeshColors()
{
	MeshData.ResetColors();
}

