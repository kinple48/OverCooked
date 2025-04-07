// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/TrayReturn.h"
#include "Components/BoxComponent.h"

// Sets default values
ATrayReturn::ATrayReturn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);
}

// Called when the game starts or when spawned
void ATrayReturn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrayReturn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

