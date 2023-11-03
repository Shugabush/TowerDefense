// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "PDPlayer.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APDPlayer();

	UFUNCTION()
		class UPDUserWidget* GetWidget() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnMouseClicked();
	virtual void OnPauseButtonPressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnTurretButtonClicked();

	UFUNCTION()
		void OnPowerGeneratorButtonClicked();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class APrisonerDefenseGameModeBase* GetGameMode() const;

	bool HasTurret() const;
	bool HasPowerGenerator() const;

private:
	void ClearTower();
	void PlaceTower();
	void UpdateTower();

	void SpawnTurret();
	void SpawnPowerGenerator();

	UPROPERTY()
		class UCameraComponent* Camera;

	UPROPERTY()
		class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDTurret> TurretReference;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDPowerGenerator> PowerGeneratorReference;

	UPROPERTY()
		class APDTower* ActiveTower;

	UPROPERTY()
		class UStaticMeshComponent* ActiveMesh;

	UPROPERTY()
		class APDTowerSlot* SelectedTowerSlot;

	UPROPERTY()
		class APrisonerDefenseGameModeBase* GameMode;

	bool IsPaused;

	UPROPERTY()
		class APDHUD* HUD;

	UPROPERTY()
		class UPDUserWidget* Widget;
};
