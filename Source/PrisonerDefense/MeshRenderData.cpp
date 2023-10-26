// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshRenderData.h"


MeshRenderData::MeshRenderData(UMeshComponent* mesh, FName colorName)
{
	for (size_t i = 0; i < mesh->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* mat = UMaterialInstanceDynamic::Create(mesh->GetMaterial(i), NULL);
		mesh->SetMaterial(i, mat);

		Materials.Add(MaterialData(mat, colorName));
	}
}

void MeshRenderData::BlendColors(FLinearColor newColor)
{
	for (size_t i = 0; i < Materials.Num(); i++)
	{
		Materials[i].BlendColor(newColor);
	}
}

void MeshRenderData::SetColors(FLinearColor newColor)
{
	for (size_t i = 0; i < Materials.Num(); i++)
	{
		Materials[i].SetColor(newColor);
	}
}

void MeshRenderData::ResetColors()
{
	for (size_t i = 0; i < Materials.Num(); i++)
	{
		Materials[i].ResetColor();
	}
}

MeshRenderData::MaterialData::MaterialData(UMaterialInstanceDynamic* material, FName colorName)
{
	Material = material;
	ColorName = colorName;
	material->GetVectorParameterValue(colorName, OriginalColor);
}

void MeshRenderData::MaterialData::BlendColor(FLinearColor newColor)
{
	Material->SetVectorParameterValue(ColorName, OriginalColor * newColor);
}

void MeshRenderData::MaterialData::SetColor(FLinearColor newColor)
{
	Material->SetVectorParameterValue(ColorName, newColor);
}

void MeshRenderData::MaterialData::ResetColor()
{
	Material->SetVectorParameterValue(ColorName, OriginalColor);
}
