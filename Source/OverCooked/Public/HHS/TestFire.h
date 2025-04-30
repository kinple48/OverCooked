// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TestFire.generated.h"

UCLASS()
class OVERCOOKED_API ATestFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* FireParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsExtinguished;
	
	UFUNCTION(BlueprintCallable)
	void Extinguish();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* firesound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAudioComponent* AudioComp;
};
