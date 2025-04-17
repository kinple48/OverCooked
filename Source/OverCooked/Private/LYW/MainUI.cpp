// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/MainUI.h"
#include "LYW/OrderUI.h"
#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"


void UMainUI::AddSalmonUI()
{
	UOrderUI* salmonUI = Cast<UOrderUI>(CreateWidget(GetWorld(), SalmonUIFactory));
	UI_Array.Add(salmonUI);
	OrderBox->AddChildToHorizontalBox(salmonUI);
	salmonUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddCucumberSushiUI()
{
	UOrderUI* CucumberUI = Cast<UOrderUI>(CreateWidget(GetWorld(), CucumberSushiUIFactory));
	UI_Array.Add(CucumberUI);
	OrderBox->AddChildToHorizontalBox(CucumberUI);
	CucumberUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddSalmonSushiUI()
{	
	UOrderUI* salmonUI = Cast<UOrderUI>(CreateWidget(GetWorld(), SalmonSushiUIFactory));
	UI_Array.Add(salmonUI);
	OrderBox->AddChildToHorizontalBox(salmonUI);
	salmonUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddMixedSushiUI()
{
	UOrderUI* MixedUI = Cast<UOrderUI>(CreateWidget(GetWorld(), MixedSushiUIFactory));
	UI_Array.Add(MixedUI);
	OrderBox->AddChildToHorizontalBox(MixedUI);
	MixedUI->AddToViewport();
	AddSpacor(SpacordWidth);
}

void UMainUI::AddSpacor(float width)
{
	// OrderBox¿¡ USpacer »ý¼º
	USpacer* spacer = NewObject<USpacer>(this);
	spacer->SetSize(FVector2D(width, 0.0f));
	OrderBox->AddChildToHorizontalBox(spacer);
}

void UMainUI::RemoveOrder(int32 idx)
{
	OrderBox->RemoveChildAt(2 * idx + 1);
	OrderBox->RemoveChildAt(2 * idx);
}
