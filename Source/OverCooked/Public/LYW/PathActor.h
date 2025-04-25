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

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineContoller")
	class USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineContoller")
	class USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineContoller")
	TSubclassOf<class AHangerActor> ActorFactory;

	class AHangerActor* Hanger;

	UPROPERTY(EditAnywhere, Category="Path")
	float LoopDuration = 20.0f;

	float ElapsedTime = 0.0f;
	float Period = LoopDuration * 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool bCanMove = true;
};
