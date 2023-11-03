// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBullet.h"
#include "PDPrisoner.h"

// Sets default values
APDBullet::APDBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &APDBullet::OnBeginOverlap);
}

// Called when the game starts or when spawned
void APDBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPrisoner != nullptr)
	{
		SetActorLocation(FMath::Lerp<FVector>(GetActorLocation(), TargetPrisoner->GetActorLocation(), DeltaTime * MovementSpeed));
	}
	else
	{
		// Destroy this object if there is no prisoner to target
		Destroy();
	}
}

void APDBullet::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// If we hit a prisoner, destroy that prisoner and this
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		prisoner->Defeated = true;
		prisoner->Destroy();
		Destroy();
	}
}

