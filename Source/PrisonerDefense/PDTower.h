// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CooldownTimer.h"
#include "MeshRenderData.h"
#include "PDTower.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDTower();

	UPROPERTY()
		class APDTowerSlot* ParentSlot;

	UFUNCTION()
		class UStaticMeshComponent* GetMesh();

	UFUNCTION()
		virtual void OnMouseEnter();

	UFUNCTION()
		virtual void OnMouseExit();

	UFUNCTION()
		virtual void OnTowerSelected();

	UFUNCTION()
		virtual void OnTowerDeselected();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		class APrisonerDefenseGameModeBase* GameMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class USphereComponent* VolumeTrigger;

	UFUNCTION()
		virtual void OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int CurrentUpgradeIndex;

	MeshRenderData MeshData;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMouseEnter"))
		void RecieveOnMouseEnter();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMouseExit"))
		void RecieveOnMouseExit();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTowerSelected"))
		void RecieveOnTowerSelected();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTowerDeselected"))
		void RecieveOnTowerDeselected();

	TArray<int> UpgradeCosts;

	UPROPERTY()
		class APDPlayer* Player;

	UPROPERTY()
		class UPDUpgradesWidget* UpgradesWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void Upgrade();

	UFUNCTION()
		virtual void OnTowerPlaced();

	int GetCurrentUpgradeIndex() const;

	// Get a copy array of upgrade costs
	TArray<int> GetUpgradeCosts() const;

	void BlendMeshColors(FLinearColor newColor);
	void SetMeshColors(FLinearColor newColor);
	void ResetMeshColors();
};
