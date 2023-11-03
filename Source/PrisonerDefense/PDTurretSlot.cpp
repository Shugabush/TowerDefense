// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurretSlot.h"
#include "PDTurret.h"
#include "PDPrisoner.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
APDTurretSlot::APDTurretSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(Box);

	FVector boxExtent = Box->GetUnscaledBoxExtent();

	Box->SetBoxExtent(FVector(boxExtent.X, boxExtent.Y, 0));
	Box->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	RootComponent = Box;

	Turret = nullptr;
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

