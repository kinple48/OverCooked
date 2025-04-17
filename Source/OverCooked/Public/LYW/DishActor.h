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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDishIngredientUI> IngredientUIFactory;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* IngreUI;

	TArray<FString> IngredientArr = {TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon")};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USushiMeshData* RollMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FString> IngredientsSet;

	void CheckIngredient();
	void AddSalmon();
	void AddSeaWeed();	
	void AddRice();
	void AddCucumber();
	
	void HideIngredients();

	UFUNCTION()
	void OnDishActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
