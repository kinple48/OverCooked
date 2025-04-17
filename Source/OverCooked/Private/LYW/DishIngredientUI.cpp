// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/DishIngredientUI.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "Components/HorizontalBox.h"


void UDishIngredientUI::NativeConstruct()
{
	Super::NativeConstruct();
	Img_Cucumber->SetVisibility(ESlateVisibility::Hidden);
	Img_Fish->SetVisibility(ESlateVisibility::Hidden);
	Img_Rice->SetVisibility(ESlateVisibility::Hidden);
	Img_Seaweed->SetVisibility(ESlateVisibility::Hidden);
	scaleBoxArr = { ScaleBox1 , ScaleBox2, ScaleBox3, ScaleBox4 };
}

//TArray<FString> IngredientArr = { TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon") };

void UDishIngredientUI::FillImg(FString ingredient)
{
	for (int32 i = 0; i < 4; i++)
	{
		if (!scaleBoxArr.IsValidIndex(i) || !scaleBoxArr[i]) 
		{
			scaleBoxArr = { ScaleBox1 , ScaleBox2, ScaleBox3, ScaleBox4 };
			UE_LOG(LogTemp, Warning, TEXT("Ingredient Arr is not valid"));
		}
		
		if(scaleBoxArr[i]->GetChildrenCount() > 0) continue;
		
		if (ingredient == TEXT("SeaWeed"))
		{
			SetImg(i, Img_Seaweed); break;
		}
		if (ingredient == TEXT("Rice"))
		{
			SetImg(i, Img_Rice); break;
		}
		if (ingredient == TEXT("Cucumber"))
		{
			SetImg(i, Img_Cucumber); break;
		}

		if (ingredient == TEXT("Salmon"))
		{
			UE_LOG(LogTemp, Warning, TEXT("idx salmon: %d"), i);
			SetImg(i, Img_Fish); break;
		}
	}
}

void UDishIngredientUI::SetImg(int32 idx, UImage* img)
{
	UE_LOG(LogTemp, Warning, TEXT("idx: %d, img is %s"), idx, *img->GetName());
	img->RemoveFromParent();
	scaleBoxArr[idx]->AddChild(img);
	img->SetVisibility(ESlateVisibility::Visible);
}
