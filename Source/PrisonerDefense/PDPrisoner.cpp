// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPrisoner.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PDPrisonerCage.h"

// Sets default values
APDPrisoner::APDPrisoner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(Capsule);

	RootComponent = Capsule;
}

// Called when the game starts or when spawned
void APDPrisoner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APDPrisoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APDPrisoner::GetGroundVelocity() const
{
	return Velocity;
}

