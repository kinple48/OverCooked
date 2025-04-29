// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHS/ChefPlayer.h"
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
	TSubclassOf<APawn> ActorFactory;
	
	class APawn* Hanger;
	UPrimitiveComponent* HangerCollision;
	
	UPROPERTY()
	TArray<AChefPlayer*> ChefPlayers;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, Category="Move")
	float MoveDuration = 15.0f;

	UPROPERTY(EditAnywhere, Category="Move")
	float SpawnInterval = 13.0f;

	UPROPERTY(EditAnywhere, Category="Move")
	float StartTime = 0.0f;

	float ElapsedTime = 0.0f;

	bool bCanMove = false;
	
	UFUNCTION()
	void SpawnHanger();

	UFUNCTION()
	void OnHangerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                     bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnHangerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
