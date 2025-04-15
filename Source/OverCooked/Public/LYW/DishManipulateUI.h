// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DishManipulateUI.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API UDishManipulateUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* AddSalmon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* AddCucumber;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* AddRice;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* AddSeaweed;
	
	class ADishActor* Dish;
	
	virtual void NativeConstruct() override;
	UFUNCTION()
	void AddSalmon_To_Dish();
	UFUNCTION()
	void AddCucumber_To_Dish();
	UFUNCTION()
	void AddRice_To_Dish();
	UFUNCTION()
	void AddSeaweed_To_Dish();
};
