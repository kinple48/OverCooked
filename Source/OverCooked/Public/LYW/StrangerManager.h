// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StrangerManager.generated.h"

UCLASS()
class OVERCOOKED_API AStrangerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrangerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class APathActor*> PathList; 

public:
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval = 13.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PeriodInterval = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartDelay;

	float SpawnCurrentTime;
	float PeriodCurrentTime;

	int32 makeCount;

	bool bCanSpawn = true;

	void HandleSpawn(float DeltaTime);
};
