// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasStove.generated.h"

UCLASS()
class OVERCOOKED_API AGasStove : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGasStove();

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
	class UArrowComponent* arrowcomp;

	UFUNCTION()
	void OnGasStoveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGasStoveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OverCookedTime = 10.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FireTime = 16.f;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* TimerWidget;

	UPROPERTY()
	bool bTimerOn = false;

	UPROPERTY()
	class UTimerUI* TimeUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATestFire> FireFactory;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> TimerUIClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CookedUIClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> OverCookedUIClass;

	void SetWidgetTo(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SnapPoint;
	
	UPROPERTY()
	class ARice* Rice;
};
