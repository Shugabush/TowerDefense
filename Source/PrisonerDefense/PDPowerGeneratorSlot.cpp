// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPowerGeneratorSlot.h"
#include "PDPowerGenerator.h"

// Sets default values
APDPowerGeneratorSlot::APDPowerGeneratorSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void APDPowerGeneratorSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDPowerGeneratorSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

