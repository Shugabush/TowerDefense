// Fill out your copyright notice in the Description page of Project Settings.


#include "PDExit.h"

#include "PDPrisoner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APDExit::APDExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &APDExit::OnBeginOverlap);
}

// Called when the game starts or when spawned
void APDExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APDExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APDExit::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		// A prisoner made it to the exit, so we lose the game
		FLatentActionInfo latentInfo;
		UGameplayStatics::LoadStreamLevel(this, TEXT("GameOver"), true, true, latentInfo);
	}
}

