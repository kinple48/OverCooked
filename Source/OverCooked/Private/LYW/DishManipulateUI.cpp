// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/DishManipulateUI.h"
#include "Components/Button.h"
#include "LYW/DishActor.h"
#include "Kismet/GameplayStatics.h"

void UDishManipulateUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	AddSalmon->OnClicked.AddDynamic(this, &UDishManipulateUI::AddSalmon_To_Dish);
	AddCucumber->OnClicked.AddDynamic(this, &UDishManipulateUI::AddCucumber_To_Dish);
	AddRice->OnClicked.AddDynamic(this, &UDishManipulateUI::AddRice_To_Dish);
	AddSeaweed->OnClicked.AddDynamic(this, &UDishManipulateUI::AddSeaweed_To_Dish);
	//MakeDish->OnClicked.AddDynamic(this, &UDishManipulateUI::MakeDishActor);

	AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), ADishActor::StaticClass());
	if (Found)
	{
		Dish = Cast<ADishActor>(Found);
		UE_LOG(LogTemp, Warning, TEXT("Found the single DishActor."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No DishActor found in the world!"));
	}
}

void UDishManipulateUI::AddSalmon_To_Dish()
{
	UE_LOG(LogTemp, Warning, TEXT("Add Salmon"));
	Dish->AddSalmon();
}

void UDishManipulateUI::AddCucumber_To_Dish()
{
	UE_LOG(LogTemp, Warning, TEXT("Add Cucumber"));
	Dish->AddCucumber();
}

void UDishManipulateUI::AddRice_To_Dish()
{
	UE_LOG(LogTemp, Warning, TEXT("Add Rice"));
	Dish->AddRice();
}

void UDishManipulateUI::AddSeaweed_To_Dish()
{
	UE_LOG(LogTemp, Warning, TEXT("Add seaweed"));
	Dish->AddSeaWeed();
}
