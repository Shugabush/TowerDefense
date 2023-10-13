// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurretSlot.h"

// Sets default values
APDTurretSlot::APDTurretSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(Box);

	FVector boxScale = Box->GetRelativeScale3D();

	Box->SetRelativeScale3D(FVector(boxScale.X, boxScale.Y, 0.01f));
	Box->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	RootComponent = Box;
}

// Called when the game starts or when spawned
void APDTurretSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDTurretSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

