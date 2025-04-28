// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LYW/OC_GameState.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* OrderBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "myUI")
	class UTextBlock* Time_txt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "myUI")
	class UProgressBar* TimeProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "myUI")
	class UTextBlock* Coin_txt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> SalmonUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> CucumberSushiUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> SalmonSushiUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> MixedSushiUIFactory;

	float SpacordWidth = 30.0f;

	UPROPERTY()
	TArray<UOrderUI*> UI_Array;

	void AddSalmonUI(const FOrderData& Order);
	void AddCucumberSushiUI(const FOrderData& Order);
	void AddSalmonSushiUI(const FOrderData& Order);
	void AddMixedSushiUI(const FOrderData& Order);
	void AddSpacor(float width);
	void RemoveOrder(int32 idx);
};
