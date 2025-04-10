// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodChecker.generated.h"

UCLASS()
class OVERCOOKED_API AFoodChecker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodChecker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	TArray<FString> IngredientArr = { TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon") };
	TArray<FString> OrderList = { TEXT("Sliced_Salmon"), TEXT("Cucumber_Roll"), TEXT("Salmon_Sushi"), TEXT("Mixed_Roll") };
	TArray<FString> OrderInfo = {TEXT("0001"), TEXT("1110"), TEXT("1101"), TEXT("1111")};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> CurrentOrder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ADishActor* DishActor;

	float currentTime = 0.0f;
	float newOrderTime = 20.0f;

	void PrintIngredient(ADishActor* dish);
	void MakeRandomOrder();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
