// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PRISONERDEFENSE_API MeshRenderData
{
private:
	class MaterialData
	{
		UMaterialInstanceDynamic* Material;
		FLinearColor OriginalColor;
		FName ColorName;

	public:
		MaterialData(UMaterialInstanceDynamic* material, FName colorName);

		void BlendColor(FLinearColor newColor);
		void SetColor(FLinearColor newColor);
		void ResetColor();
	};
	TArray<MaterialData> Materials;

public:
	MeshRenderData();
	MeshRenderData(UMeshComponent* mesh, FName colorName);
	~MeshRenderData();

	void BlendColors(FLinearColor newColor);
	void SetColors(FLinearColor newColor);
	void ResetColors();
};
