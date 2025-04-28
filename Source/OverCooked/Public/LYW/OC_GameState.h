// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OC_GameState.generated.h" 

/**
 * 
 */

USTRUCT(BlueprintType)
struct FOrderData
{
	GENERATED_BODY()

	bool operator==(const FOrderData& Other) const
	{
		return OrderID == Other.OrderID;
	}


	UPROPERTY()
	FString OrderID;

	UPROPERTY()
	float StartTime; // 서버에서 생성된 GameTimeSeconds

	UPROPERTY()
	float Duration; // MissionTime

	UPROPERTY()
	int32 Price;
};

UCLASS()
class OVERCOOKED_API AOC_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AOC_GameState();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	float RemainingTime;
	
	UPROPERTY(Replicated)
	int32 coin = 0;

	float GameTime = 180.0f;

	UPROPERTY(Replicated)
	TArray<FOrderData> OrderList;

	TArray<FString> IngredientArr = { TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon") };
	TArray<FString> OrderName = { TEXT("Sliced_Salmon"), TEXT("Cucumber_Roll"), TEXT("Salmon_Sushi"), TEXT("Mixed_Roll") };
	TArray<FString> OrderInfo = { TEXT("0001"), TEXT("1110"), TEXT("1101"), TEXT("1111") };
	TArray<float> OrderDuration = { 30.0f, 50.0f, 50.0f, 60.0f };
	TArray<int32> OrderPrice = { 1, 1, 1, 1 };
	
	void AddCoin(int32 Price);
	FOrderData MakeRandomOrder();
	void FinishGame();
};
