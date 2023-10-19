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
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		LookAtTargets.Add(prisoner);
	}
}

void APDTurretSlot::OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APDPrisoner* prisoner = Cast<APDPrisoner>(OtherActor);
	if (prisoner != nullptr)
	{
		LookAtTargets.Remove(prisoner);
	}
}

// Called every frame
void APDTurretSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

APDPrisoner* APDTurretSlot::GetClosestTarget() const
{
	float closestDst = 10000;
	int targetIndex = -1;

	if (Turret == nullptr) { return nullptr; }

	for (size_t i = 0; i < LookAtTargets.Num(); i++)
	{
		APDPrisoner* target = LookAtTargets[i];

		float dstToTarget = FVector::Dist(Turret->GetActorLocation(), target->GetActorLocation());
		if (dstToTarget < closestDst)
		{
			closestDst = dstToTarget;
			targetIndex = i;
		}
	}
	if (LookAtTargets.IsValidIndex(targetIndex))
	{
		return LookAtTargets[targetIndex];
	}
	else
	{
		return nullptr;
	}
}

