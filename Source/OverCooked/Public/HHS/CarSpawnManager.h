// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LYW/CarActor.h"
#include "CarSpawnManager.generated.h"

class ACar;

UCLASS()
class OVERCOOKED_API ACarSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACarSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<ACarActor> CarClass;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	class ACarActor* CarActor;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnInterval; // 차량 생성 주기

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float StartTime; // 게임 시작 후 언제부터 움직이기를 시작할 지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* SpawnArrow;

	FTimerHandle SpawnTimerHandle;

	//UPROPERTY(EditAnywhere, Category = "Sound")
	//USoundBase* CarSpawnSound;

	
	UFUNCTION()
	void SpawnCar();
};
