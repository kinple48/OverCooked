// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SushiMeshData.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API USushiMeshData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> RollMeshArr;
	
	void SetMesh_CucumberSushi(AActor* Dish);
	void SetMesh_SalmonSushi(AActor* Dish);
	void SetMesh_MixedSushi(AActor* Dish);
};
