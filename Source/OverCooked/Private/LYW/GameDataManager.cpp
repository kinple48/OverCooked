// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/GameDataManager.h"
#include "LYW/MainUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "LYW/OrderUI.h"
#include "Net/UnrealNetwork.h"
#include "Components/HorizontalBox.h"
#include "HHS/EndGameUI.h"

// Sets default values
AGameDataManager::AGameDataManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//NetUpdateFrequency = 10.0f;
}

// Called when the game starts or when spawned
void AGameDataManager::BeginPlay()
{
	Super::BeginPlay();

	mainUI = Cast<UMainUI>(CreateWidget(GetWorld(), UIFactory));
	GameState = GetWorld()->GetGameState<AOC_GameState>();
	
	if (mainUI)
	{
		mainUI->AddToViewport();
		bUIReady = true;
		mainUI->Coin_txt->SetText(FText::FromString(FString::FromInt(GameState->coin)));
	}


	if (HasAuthority() && GameState)
	{
		for (int32 i = 0; i < 2; i++)
		{
			auto str = GameState -> MakeRandomOrder();
		}

	}

	for (const FOrderData& OrderData : GameState->OrderList)
	{
		AddOrderUI(OrderData);
	}
}

// Called every frame
void AGameDataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (!mainUI)
	{
		mainUI = Cast<UMainUI>(CreateWidget(GetWorld(), UIFactory));
		if (mainUI)
		{
			mainUI->AddToViewport();
			bUIReady = true;
		}
	}*/

	if (HasAuthority()) // ����
	{
		OrdercurrentTime += DeltaTime;
		UpdatecurrentTime += DeltaTime;
		if (!GameState)
		{
			GameState = GetWorld()->GetGameState<AOC_GameState>();
		}
		
		if (OrdercurrentTime > newOrderTime && GameState->OrderList.Num() < 5)
		{
			FOrderData Order = GameState->MakeRandomOrder();
			OrdercurrentTime = 0.0f;
			MulticastRPC_AddOrderUI(Order);
		}


		if (UpdatecurrentTime > UIUpdateTIme)
		{
			MulticastRPC_SetTimePercent(GameState->RemainingTime, GameState->GameTime);
			UpdatecurrentTime = 0.0f;
			/*float Now = GetWorld()->GetTimeSeconds();
			for (int32 i = 0; i < GameState->OrderList.Num(); i++)
			{
				FOrderData& menu = GameState->OrderList[i];
				if (Now > menu.StartTime + menu.Duration)
				{
					menu.StartTime = Now;
					AddCoin(menu.Price * -1);
					MulticastRPC_SetIndividualOrderProgress(i, 1.0f);

				}
				else
				{
					MulticastRPC_SetIndividualOrderProgress(i, 1.0f - (Now - menu.StartTime) / menu.Duration);

				}
			}*/
		}

	}
}

void AGameDataManager::MulticastRPC_SetTimePercent_Implementation(float currentTime, float GameTime)
{
	if (bUIReady && mainUI && mainUI->Time_txt && mainUI->TimeProgressBar)
	{
		min = int32(currentTime) / 60;
		sec = int32(currentTime) % 60;
		TimeStr = FString::Printf(TEXT("%02d:%02d"), min, sec);
		mainUI->Time_txt->SetText(FText::FromString(TimeStr));
		mainUI->TimeProgressBar->SetPercent(currentTime / GameTime);
	}
	if (HasAuthority() && currentTime <= 0.0f)
	{
		//EndGame();
		int32 FinalScore = GameState->coin;
		MulticastRPC_EndUI(FinalScore);
		//GameState->FinishGame();
	}
}

void AGameDataManager::MulticastRPC_SetCoinUI_Implementation(int32 currnet_coin)
{
	if (mainUI && bUIReady)
	{
		mainUI->Coin_txt->SetText(FText::FromString(FString::FromInt(currnet_coin)));
	}
}


void AGameDataManager::AddOrderUI(const FOrderData& Order)
{
	if (mainUI && bUIReady)
	{
		if (Order.OrderID == TEXT("0001"))
		{
			mainUI->AddSalmonUI(Order);
		}
		else if (Order.OrderID == TEXT("1110"))
		{
			mainUI->AddCucumberSushiUI(Order);
		}
		else if (Order.OrderID == TEXT("1101"))
		{
			mainUI->AddSalmonSushiUI(Order);
		}
		else
		{
			mainUI->AddMixedSushiUI(Order);
		}
	}
}


void AGameDataManager::MulticastRPC_AddOrderUI_Implementation(const FOrderData& Order)
{
	AddOrderUI(Order); // UI �߰�
}

//void AGameDataManager::MulticastRPC_RemoveOderUI_Implementation(int32 index)
//{
//	if (mainUI)
//	{
//		mainUI->RemoveOrder(index);
//	}
//}
//void AGameDataManager::MulticastRPC_SetIndividualOrderProgress_Implementation(int32 index, float percent)
//{
//	if (bUIReady && mainUI && mainUI->UI_Array.IsValidIndex(index))
//	{
//		UOrderUI* currentUI = mainUI->UI_Array[index];
//		if (IsValid(currentUI) && IsValid(currentUI->ProgressBar))
//		{
//			currentUI->SetPercent(percent);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("UOrderUI 또는 ProgressBar가 유효하지 않습니다! index: %d"), index);
//		}
//	}
//}

void AGameDataManager::MulticastRPC_EndUI_Implementation(int32 FinalScore)
{
	UE_LOG(LogTemp, Warning, TEXT("게임 종료!"));

	if (mainUI)
	{
		mainUI->RemoveFromParent();
	}
	EndGame(FinalScore);
}

void AGameDataManager::CheckOrder(const FString& OrderStr)
{
	float min_percent = 2.0f;
	int32 min_idx = -1;
	int price = 0;
	for (int32 i = 0; i < GameState->OrderList.Num(); i++)
	{
		const FOrderData& OrderData = GameState->OrderList[i];

		if (OrderStr == OrderData.OrderID)
		{
			if (mainUI && mainUI->UI_Array.IsValidIndex(i))
			{
				UOrderUI* ord = Cast<UOrderUI>(mainUI->UI_Array[i]);

				if (ord && ord->TimePercent < min_percent)
				{
					min_percent = ord->TimePercent;
					min_idx = i;
					price = OrderData.Price;
				}
			}
		}
	}

	if (HasAuthority())
	{
		if (min_idx != -1)
		{
			GameState->OrderList.RemoveAt(min_idx);
			//mainUI->RemoveOrder(min_idx);
			MulticastRPC_RemoveOderUI(min_idx);
			AddCoin(price);

			if (GameState->OrderList.Num() < 2)
			{
				FOrderData Order = GameState->MakeRandomOrder();
				MulticastRPC_AddOrderUI(Order);
			}
		}
	}
	
	/*else
	{
		AddCoin(-10);
	}*/
}

void AGameDataManager::MulticastRPC_RemoveOderUI_Implementation(int32 index)
{
	if (mainUI)
	{
		mainUI->RemoveOrder(index);

		for (int32 i = 0; i < mainUI->UI_Array.Num(); i++)
		{
			UOrderUI* currentUI = mainUI->UI_Array[i];
			if (currentUI)
			{
				currentUI->myIndex = i;
			}
		}
	}
}

void AGameDataManager::AddCoin(int32 Price)
{
	if (HasAuthority())
	{
		GameState->coin += Price;
		MulticastRPC_SetCoinUI(GameState->coin);
	}
}


void AGameDataManager::EndGame(int32 FinalScore)
{
	int32 StarCount = 0;
	
	//// 테스트용
	//if (FinalScore <= 0)
	//	StarCount = 3;

	if (FinalScore >= 15)
		StarCount = 3;
	else if (FinalScore >= 10)
		StarCount = 2;
	else if (FinalScore >= 5)
		StarCount = 1;
	else
		StarCount = 0;

	// EndGameUI 생성
	if (EndGameUI == nullptr)
	{
		EndGameUI = Cast<UEndGameUI>(CreateWidget(GetWorld(), EndGameUIFactory));
	}

	if (EndGameUI)
	{
		EndGameUI->AddToViewport();
		EndGameUI->SetupResult(FinalScore, StarCount);
	}
}

void AGameDataManager::FinishOrderTime(int32 myIndex)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("You can't submit menu: %d"), myIndex);
		if (GameState && GameState->OrderList.IsValidIndex(myIndex))
		{
			GameState->OrderList[myIndex].StartTime = GetWorld()->GetTimeSeconds();
			AddCoin(GameState->OrderList[myIndex].Price * -1);
		}
	}
}

void AGameDataManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameDataManager, OrdercurrentTime);
	DOREPLIFETIME(AGameDataManager, UpdatecurrentTime);
}
