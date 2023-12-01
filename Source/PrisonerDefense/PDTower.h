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

	UPROPERTY(VisibleAnywhere)
		class APDTowerSlot* ParentSlot;

	UFUNCTION()
		class UStaticMeshComponent* GetMesh();

	UFUNCTION()
		virtual void OnMouseEnter();

	UFUNCTION()
		virtual void OnMouseExit();

	UFUNCTION()
		virtual void OnTowerSelected(APDTower* PreviouslySelectedTower);

	UFUNCTION()
		virtual void OnTowerDeselected(APDTower* NewSelectedTower);

	UFUNCTION()
		virtual FText GetUpgradeDescription() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnRoundStarted();

	UFUNCTION()
		virtual void OnRoundEnded();

	UPROPERTY()
		class APrisonerDefenseGameModeBase* GameMode;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class USphereComponent* VolumeTrigger;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		class USphereComponent* SelectionRadius;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* RangeIndicator;

	UFUNCTION()
		virtual void OnVolumeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnVolumeTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int CurrentUpgradeIndex;

	// Starting radius for volume trigger
	float VolumeTriggerRadius;
	// Starting scale for range indicator
	FVector RangeIndicatorScale;

	MeshRenderData MeshData;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMouseEnter"))
		void RecieveOnMouseEnter();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMouseExit"))
		void RecieveOnMouseExit();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTowerSelected"))
		void RecieveOnTowerSelected(APDTower* PreviouslySelectedTower);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Upgrade"))
		void RecieveUpgrade();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTowerDeselected"))
		void RecieveOnTowerDeselected(APDTower* NewSelectedTower);

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
