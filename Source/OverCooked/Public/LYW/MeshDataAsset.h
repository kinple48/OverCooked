#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

UCLASS(BlueprintType)
class OVERCOOKED_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> RollMeshArr;
	
	void LoadMeshFromPath();
	void SetMesh_CucumberSushi(AActor* Dish);
	void SetMesh_SalmonSushi(AActor* Dish);
	void SetMesh_MixedSushi(AActor* Dish);
};