// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CooldownTimer.h"
#include "MeshRenderData.h"
#include "PDTower.generated.h"

USTRUCT(BlueprintType)
// Tower upgrade blueprint
struct FTowerUpgrade
{
	GENERATED_BODY()
public:
	int GetPowerCost() const;
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		int PowerCost = 100;
};

UCLASS()
class PRISONERDEFENSE_API APDTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		class APrisonerDefenseGameModeBase* GameMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FTowerUpgrade> Upgrades;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		class UWidgetComponent* Widget;

	// Widget property as upgrades widget
	UPROPERTY()
		class UPDUpgradesWidget* UpgradesWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class UBoxComponent* VolumeTrigger;

	UFUNCTION()
		virtual void OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	MeshRenderData MeshData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void Upgrade();

};
