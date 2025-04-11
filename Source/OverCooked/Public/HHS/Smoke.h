// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Smoke.generated.h"

UCLASS()
class OVERCOOKED_API ASmoke : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmoke();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SmokeMesh;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicSmoke;

	UPROPERTY(EditAnywhere)
	float SmokeSpeed = 50.f;

	UPROPERTY(EditAnywhere)
	float LifeTime = 1.0f;

	UPROPERTY(EditAnywhere)
	float FadeStartTime = 1.0f;

	float ElapsedTime = 0.0f;


	
};
