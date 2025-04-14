// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrayReturn.generated.h"

UCLASS()
class OVERCOOKED_API ATrayReturn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrayReturn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* SpawnArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADirtyDish> DishFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DishCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float curTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxTime = 5.f;
};
