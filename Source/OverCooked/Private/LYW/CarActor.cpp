// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/CarActor.h"

// Sets default values
ACarActor::ACarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACarActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

