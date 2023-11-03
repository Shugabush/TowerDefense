// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CooldownTimer.h"
#include "PDTower.h"

#include "GameFramework/Actor.h"
#include "PDPowerGenerator.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDPowerGenerator : public APDTower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDPowerGenerator();

	class UStaticMeshComponent* GetMesh() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BlendMeshColors(FLinearColor newColor);
	void SetMeshColors(FLinearColor newColor);
	void ResetMeshColors();

	void OnPowerGeneratorPlaced();

	virtual void Upgrade() override;

	UPROPERTY()
		class APDTowerSlot* ParentSlot;

	UPROPERTY()
		class APDPlayer* Player;
	
private:
	FCooldownTimer Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int PowerPerSecond = 2;
};
