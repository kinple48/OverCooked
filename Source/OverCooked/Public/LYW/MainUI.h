// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> SalmonUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> CucumberSushiUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> SalmonSushiUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UOrderUI> MixedSushiUIFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Coin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimePercent;

	float SpacorWidth = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString TimePrint;

	TArray<UOrderUI*> UI_Array;
	void AddSalmonUI();
	void AddCucumberSushiUI();
	void AddSalmonSushiUI();
	void AddMixedSushiUI();
	void AddSpacor(float width);
	void RemoveOrder(int32 idx);
};
