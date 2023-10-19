// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MeshRenderData.h"

#include "GameFramework/Actor.h"
#include "PDTurret.generated.h"

UCLASS()
class PRISONERDEFENSE_API APDTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDTurret();

	class UStaticMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float RotationLerpSpeed = 5;

	MeshRenderData MeshData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BlendMeshColors(FLinearColor newColor);
	void SetMeshColors(FLinearColor newColor);
	void ResetMeshColors();

	FQuat TargetRotation;

	TArray<AActor*> LookAtTargets;

	UFUNCTION()
	AActor* GetClosestTarget() const;
};
