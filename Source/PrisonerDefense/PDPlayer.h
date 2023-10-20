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

	bool HasTurret();
	bool HasPowerGenerator();

private:
	void ClearTurret();
	void SpawnTurret();
	void PlaceTurret();
	void UpdateTurret();

	void ClearPowerGenerator();
	void SpawnPowerGenerator();
	void PlacePowerGenerator();
	void UpdatePowerGenerator();

	class UCameraComponent* Camera;
	class APlayerController* Controller;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDTurret> TurretReference;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TSubclassOf<class APDPowerGenerator> PowerGeneratorReference;

	class APDTurret* ActiveTurret;
	class APDPowerGenerator* ActivePowerGenerator;

	class AActor* ActiveObject;
	class UStaticMeshComponent* ActiveMesh;

	class APDTurretSlot* SelectedTurretSlot;
	class APDPowerGenerator* SelectedPowerGeneratorSlot;

	bool IsPaused;

	class APDHUD* HUD;
	class UPDUserWidget* Widget;
};
