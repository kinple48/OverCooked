// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/OC_GameState.h"
#include "Net/UnrealNetwork.h"

AOC_GameState::AOC_GameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOC_GameState::BeginPlay()
{
	Super::BeginPlay();
	RemainingTime = GameTime;
	
}


void AOC_GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RemainingTime -= DeltaTime;
	float currentTime = GetWorld()->GetTimeSeconds();
}

void AOC_GameState::AddCoin(int32 Price)
{
	coin += Price;
}


FOrderData AOC_GameState::MakeRandomOrder()
{
	int32 randomMenu = FMath::RandRange(0, 3);
	//CurrentOrder.Add(OrderInfo[randomMenu]);

	FOrderData NewOrder;
	NewOrder.OrderID = OrderInfo[randomMenu];
	NewOrder.StartTime = GetWorld()->GetTimeSeconds();
	NewOrder.Duration = OrderDuration[randomMenu];
	NewOrder.Price = OrderPrice[randomMenu];
	OrderList.Add(NewOrder);
	return NewOrder;
}

void AOC_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOC_GameState, RemainingTime);
	DOREPLIFETIME(AOC_GameState, coin);
	DOREPLIFETIME(AOC_GameState, OrderList);
}
