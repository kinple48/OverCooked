// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodSpawnManager.generated.h"

UCLASS()
class OVERCOOKED_API AFoodSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MakeTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ARice>  RiceFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASeaWeed>  SeaWeedFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* SpawnArrow;

	bool state = true;

	void MakeRice();
	void MakeSeaWeed();
};
