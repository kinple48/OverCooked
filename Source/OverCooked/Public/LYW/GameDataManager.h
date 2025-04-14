// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameDataManager.generated.h"

UCLASS()
class OVERCOOKED_API AGameDataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameDataManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	int32 coin;
	float GameTime = 300.0f;
	float CurrentTime = GameTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TimeStr;

	int32 min;
	int32 sec;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> UIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMainUI* mainUI;

public:
	void AddCoin(int32 Coin_Score);
	void SetTimePercent();


public: 
	TArray<FString> IngredientArr = { TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon") };
	TArray<FString> OrderList = { TEXT("Sliced_Salmon"), TEXT("Cucumber_Roll"), TEXT("Salmon_Sushi"), TEXT("Mixed_Roll") };
	TArray<FString> OrderInfo = {TEXT("0001"), TEXT("1110"), TEXT("1101"), TEXT("1111")};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> CurrentOrder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ADishActor* DishActor;



	float currentTime = 0.0f;
	float newOrderTime = 20.0f;

	void MakeRandomOrder();
	void CheckOder(FString order);

};
