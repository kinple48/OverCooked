// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/MeshDataAsset.h"
#include "LYW/DishActor.h"

void UMeshDataAsset::SetMesh_CucumberSushi(AActor* Dish)
{
	if (ADishActor* dishActor = Cast<ADishActor>(Dish))
	{
		
		if(RollMeshArr[0] && dishActor->FoodMesh)
			dishActor->FoodMesh->SetStaticMesh(RollMeshArr[0]);
	}
}

void UMeshDataAsset::SetMesh_SalmonSushi(AActor* Dish)
{
	if (ADishActor* dishActor = Cast<ADishActor>(Dish))
	{
		if (!dishActor->FoodMesh)
			UE_LOG(LogTemp, Error, TEXT("FoodMesh is nullptr"));
		if (RollMeshArr[1] && dishActor->FoodMesh)
			dishActor->FoodMesh->SetStaticMesh(RollMeshArr[1]);
	}
}

void UMeshDataAsset::SetMesh_MixedSushi(AActor* Dish)
{
	if (ADishActor* dishActor = Cast<ADishActor>(Dish))
	{
		if (RollMeshArr[2] && dishActor->FoodMesh)
			dishActor->FoodMesh->SetStaticMesh(RollMeshArr[2]);
	}
}
