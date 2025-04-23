// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fish.generated.h"

UCLASS()
class OVERCOOKED_API AFish : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshcomp;

	UFUNCTION()
	void OnFishBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFishEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool move = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed = 100.f;

	FTimerHandle ScaleTimerHandle;
	float ScaleTimeElapsed;
	float EaseOutSine(float x);
	void StartScaleDown();
	void UpdateScale();

	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    bool bCooked;

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ChopFish();

	UPROPERTY(EditDefaultsOnly, Category = "Chop")
    UStaticMesh* ChoppedMesh;

	UFUNCTION(Server, Reliable)
    void ServerRPC_ChopFish();
};
