// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTurretSlot.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
APDTurretSlot::APDTurretSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	VolumeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("VolumeTrigger"));

	VolumeTrigger->OnComponentBeginOverlap.AddDynamic(this, &APDTurretSlot::OnVolumeTriggerBeginOverlap);
	VolumeTrigger->OnComponentEndOverlap.AddDynamic(this, &APDTurretSlot::OnVolumeTriggerEndOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(Box);
	VolumeTrigger->AttachTo(Box);

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

void APDTurretSlot::OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, OverlappedComponent->GetName());
	GEngine->AddOnScreenDebugMessage(2, 1, FColor::Blue, OtherComp->GetName());

	if (Turret != nullptr)
	{
		Turret->LookAtTargets.Add(OtherActor);
	}
}

void APDTurretSlot::OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Turret != nullptr)
	{
		Turret->LookAtTargets.Remove(OtherActor);
	}
}

// Called every frame
void APDTurretSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

