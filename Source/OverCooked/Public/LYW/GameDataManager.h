// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LYW/OC_GameState.h"
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

	UPROPERTY()
	class AOC_GameState* GameState;

////////////////// TIME ////////////////////////
public:
	
	UPROPERTY(Replicated)
	float OrdercurrentTime = 0.0f;

	UPROPERTY(Replicated)
	float UpdatecurrentTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UIUpdateTIme = 0.5f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float newOrderTime = 20.0f;

	int32 min;
	int32 sec;
	FString TimeStr;

	void SetTimePercent(float time);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetTimePercent(float currentTime, float GameTime);

public:

	void AddCoin(int32 Price);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetCoinUI(int32 currnet_coin);

////////////////// UI ////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> UIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMainUI* mainUI;
	
	bool bUIReady = false;
	
	void AddOrderUI(const FOrderData& Order);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_AddOrderUI(const FOrderData& Order);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RemoveOderUI(int32 index);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetIndividualOrderProgress(int32 index, float percent);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_EndUI(int32 FinalScore);

	
	UPROPERTY(VisibleAnywhere)
	int32 ArrayNum = 0;
	
////////////////// Order ////////////////////////
public: 
	//TArray<FString> IngredientArr = { TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon") };
	//TArray<FString> OrderList = { TEXT("Sliced_Salmon"), TEXT("Cucumber_Roll"), TEXT("Salmon_Sushi"), TEXT("Mixed_Roll") };
	//TArray<FString> OrderInfo = {TEXT("0001"), TEXT("1110"), TEXT("1101"), TEXT("1111")};
	//TArray<int32> OrderPrice = { 1, 1, 1, 1 };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ADishActor* DishActor;

	void CheckOrder(const FString& OrderStr);

	void EndGame(int32 FinalScore);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UEndGameUI> EndGameUIFactory;

	//void FinishOrderTime(int32 myIndex);

private:
	UEndGameUI* EndGameUI;
};