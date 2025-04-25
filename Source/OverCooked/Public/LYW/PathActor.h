// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathActor.generated.h"

UCLASS()
class OVERCOOKED_API APathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineContoller")
	class USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineContoller")
	class USplineComponent* Spline;
};
