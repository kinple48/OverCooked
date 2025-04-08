// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/ExtinguisherActor.h"

// Sets default values
AExtinguisherActor::AExtinguisherActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AExtinguisherActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExtinguisherActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExtinguisherActor::ActivateExtinguisher()
{
}

