// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/RollMeshDataAsset.h"
#include "LYW/DishActor.h"

void URollMeshDataAsset::SetMesh_CucumberSushi(AActor* Dish)
{
	if (ADishActor* dishActor = Cast<ADishActor>(Dish))
	{
		if (RollMeshArr[0] && dishActor->FoodMesh)
			dishActor->FoodMesh->SetStaticMesh(RollMeshArr[0]);
	}
}

void URollMeshDataAsset::SetMesh_SalmonSushi(AActor* Dish)
{
	if (ADishActor* dishActor = Cast<ADishActor>(Dish))
	{
		if (RollMeshArr[1] && dishActor->FoodMesh)
			dishActor->FoodMesh->SetStaticMesh(RollMeshArr[1]);
	}
}

void URollMeshDataAsset::SetMesh_MixedSushi(AActor* Dish)
{
	if (ADishActor* dishActor = Cast<ADishActor>(Dish))
	{
		if (RollMeshArr[2] && dishActor->FoodMesh)
			dishActor->FoodMesh->SetStaticMesh(RollMeshArr[2]);
	}
}
