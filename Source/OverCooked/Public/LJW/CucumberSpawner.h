// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CucumberSpawner.generated.h"

UCLASS()
class OVERCOOKED_API ACucumberSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACucumberSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACucumber> CucumberFactory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* SpawnArrow;

	bool state = true;

	void MakeCucumber();
};
