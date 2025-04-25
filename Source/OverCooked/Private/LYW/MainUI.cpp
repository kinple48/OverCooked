// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/MainUI.h"
#include "LYW/OrderUI.h"
#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"


void UMainUI::AddSalmonUI(const FOrderData& Order)
{
	UOrderUI* salmonUI = Cast<UOrderUI>(CreateWidget(GetWorld(), SalmonUIFactory));
	//salmonUI->InitUI(Order.StartTime, Order.Duration, Order.Price);
	UI_Array.Add(salmonUI);
	OrderBox->AddChildToHorizontalBox(salmonUI);
	salmonUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddCucumberSushiUI(const FOrderData& Order)
{
	UOrderUI* CucumberUI = Cast<UOrderUI>(CreateWidget(GetWorld(), CucumberSushiUIFactory));
	//CucumberUI->InitUI(Order.StartTime, Order.Duration, Order.Price);
	UI_Array.Add(CucumberUI);
	OrderBox->AddChildToHorizontalBox(CucumberUI);
	CucumberUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddSalmonSushiUI(const FOrderData& Order)
{
	UOrderUI* salmonUI = Cast<UOrderUI>(CreateWidget(GetWorld(), SalmonSushiUIFactory));
	//salmonUI->InitUI(Order.StartTime, Order.Duration, Order.Price);
	UI_Array.Add(salmonUI);
	OrderBox->AddChildToHorizontalBox(salmonUI);
	salmonUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddMixedSushiUI(const FOrderData& Order)
{
	UOrderUI* MixedUI = Cast<UOrderUI>(CreateWidget(GetWorld(), MixedSushiUIFactory));
	//MixedUI->InitUI(Order.StartTime, Order.Duration, Order.Price);
	UI_Array.Add(MixedUI);
	OrderBox->AddChildToHorizontalBox(MixedUI);
	MixedUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddSpacor(float width)
{
	// OrderBox에 USpacer 생성
	USpacer* spacer = NewObject<USpacer>(this);
	spacer->SetSize(FVector2D(width, 0.0f));
	OrderBox->AddChildToHorizontalBox(spacer);
}

void UMainUI::RemoveOrder(int32 idx)
{
	OrderBox->RemoveChildAt(2 * idx + 1);
	OrderBox->RemoveChildAt(2 * idx);
}
