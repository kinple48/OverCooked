// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/GameDataManager.h"
#include "LYW/MainUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

// Sets default values
AGameDataManager::AGameDataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameDataManager::BeginPlay()
{
	Super::BeginPlay();
	mainUI = Cast<UMainUI>(CreateWidget(GetWorld(), UIFactory));
	if (mainUI)
	{
		mainUI->AddToViewport();
	}

	for (int32 i = 0; i < 2; i++)
	{
		MakeRandomOrder();
	}
	currentTime = 0.0f;
}

// Called every frame
void AGameDataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentTime -= DeltaTime;
	SetTimePercent();

	currentTime += DeltaTime;

	if (currentTime > newOrderTime && CurrentOrder.Num() < 5)
	{
		MakeRandomOrder();
	}

}

void AGameDataManager::AddCoin(int32 Coin_Score)
{
	coin+= Coin_Score;
	if (mainUI)
	{
		mainUI->Coin = coin;
	}
}

void AGameDataManager::SetTimePercent()
{
	min = int32(CurrentTime) / 60;
	sec = int32(CurrentTime) % 60;
	TimeStr = FString::Printf(TEXT("%02d:%02d"), min, sec);

	if (mainUI)
	{
		mainUI->TimePercent = CurrentTime / GameTime;
		mainUI->TimePrint = TimeStr;
	}
}
//	TArray<FString> OrderList = { TEXT("Sliced_Salmon"), TEXT("Cucumber_Roll"), TEXT("Salmon_Sushi"), TEXT("Mixed_Roll") };
//TArray<FString> OrderInfo = { TEXT("0001"), TEXT("1110"), TEXT("1101"), TEXT("1111") };


void AGameDataManager::MakeRandomOrder()
{
	int32 menu = FMath::RandRange(0, 3);
	CurrentOrder.Add(OrderInfo[menu]);
	currentTime = 0.0f;

	if (OrderInfo[menu] == TEXT("0001"))
	{
		mainUI->AddSalmonUI();
	}
	else if (OrderInfo[menu] == TEXT("1110"))
	{
		mainUI->AddCucumberSushiUI();
	}
	else if (OrderInfo[menu] == TEXT("1101"))
	{
		mainUI->AddSalmonSushiUI();
	}
	else
	{
		mainUI->AddMixedSushiUI();
	}
	
}

void AGameDataManager::CheckOder(FString order)
{
	for (int32 i = 0; i < CurrentOrder.Num(); i++)
	{
		if (order == CurrentOrder[i])
		{
			CurrentOrder.RemoveAt(i);

			mainUI->RemoveOrder(i);
			AddCoin(1);
			if (CurrentOrder.Num() < 2)
			{
				MakeRandomOrder();
			}
			UE_LOG(LogTemp, Warning, TEXT("%s"), *order);
			break;
		}
	}
}

