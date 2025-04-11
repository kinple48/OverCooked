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
	class UUniformGridPanel* OrderGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Coin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimePercent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString TimePrint;
};
