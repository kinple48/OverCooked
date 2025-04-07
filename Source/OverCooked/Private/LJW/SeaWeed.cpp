// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/SeaWeed.h"

// Sets default values
ASeaWeed::ASeaWeed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASeaWeed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASeaWeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

