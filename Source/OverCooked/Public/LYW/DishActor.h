// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DishActor.generated.h"

UCLASS()
class OVERCOOKED_API ADishActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADishActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintReadWrite)
	class USceneComponent* IngredientComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* DishMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SalmonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SeaWeedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* RiceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* CucumberMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* FoodMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	TArray<FString> IngredientArr = {TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon")};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMeshDataAsset* RollMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FString> IngredientsSet;

	UFUNCTION(BlueprintCallable)
	void CheckIngredient();

	UFUNCTION(BlueprintCallable)
	void AddSalmon();

	UFUNCTION(BlueprintCallable)
	void AddSeaWeed();

	UFUNCTION(BlueprintCallable)
	void AddRice();

	UFUNCTION(BlueprintCallable)
	void AddCucumber();
	
	void HideIngredients();
	void ChangeFoodMesh();
};
