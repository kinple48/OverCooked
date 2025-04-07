// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/FoodSpawnManager.h"

// Sets default values
AFoodSpawnManager::AFoodSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoodSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

