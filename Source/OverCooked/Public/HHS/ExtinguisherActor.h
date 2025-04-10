// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtinguisherActor.generated.h"

UCLASS()
class OVERCOOKED_API AExtinguisherActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtinguisherActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* FireEffect;
	
	void ActivateExtinguisher();
	void DeactivateExtinguisher();
	void SprayOnce();
	
	FTimerHandle DeactivateTimerHandle;
	FTimerHandle SprayTimerHandle;

	UPROPERTY(EditAnywhere, Category="Grab")
	float SphereRadius = 30.0f;

};
