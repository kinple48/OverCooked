// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DishIngredientUI.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED_API UDishIngredientUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UImage* Img_Seaweed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UImage* Img_Fish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UImage* Img_Cucumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UImage* Img_Rice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UScaleBox* ScaleBox1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UScaleBox* ScaleBox2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UScaleBox* ScaleBox3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UScaleBox* ScaleBox4;

	TArray<UScaleBox*> scaleBoxArr;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void FillImg(FString ingredient);

	void SetImg(int32 idx, UImage* img);
};

