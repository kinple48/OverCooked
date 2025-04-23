// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuttingBoard.generated.h"

UCLASS()
class OVERCOOKED_API ACuttingBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACuttingBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshcomp1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshcomp2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SnapPoint;

	UPROPERTY(Replicated)
	bool bSnap = true;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool OnDish = false;

	UFUNCTION()
	void OnCuttingBoardEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCuttingBoardBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Chop")
	AActor* KnifeOnBoard;

	UPROPERTY(Replicated, BlueprintReadOnly)
	class ACucumber* cucumber;

	UPROPERTY(Replicated, BlueprintReadOnly)
	class AFish* fish;
};
