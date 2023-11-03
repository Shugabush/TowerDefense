// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"
#include "PDTower.h"

#include "GameFramework/Actor.h"
#include "PDTurret.generated.h"

USTRUCT(BlueprintType)
// Turret upgrade blueprint
struct FTurretUpgrade : public FTowerUpgrade
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FCooldownTimer NewBulletSpawnTimer;
};

UCLASS()
class PRISONERDEFENSE_API APDTurret : public APDTower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDTurret();

	class UStaticMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		float RotationLerpSpeed = 5;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BlendMeshColors(FLinearColor newColor);
	void SetMeshColors(FLinearColor newColor);
	void ResetMeshColors();

	void OnTurretPlaced();

	virtual void Upgrade() override;

	UPROPERTY()
		class APDTurretSlot* ParentSlot;

	FQuat TargetRotation;

	virtual void OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDBullet> BulletBlueprint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		FCooldownTimer BulletSpawnTimer;

	UPROPERTY()
		TArray<APDPrisoner*> LookAtTargets;

	UFUNCTION()
		class APDPrisoner* GetClosestTarget() const;
};
