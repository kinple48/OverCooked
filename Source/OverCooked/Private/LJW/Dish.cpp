// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Dish.h"

// Sets default values
ADish::ADish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADish::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

