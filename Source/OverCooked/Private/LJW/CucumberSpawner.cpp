// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/CucumberSpawner.h"

#include "Components/ArrowComponent.h"
#include "LJW/Cucumber.h"

// Sets default values
ACucumberSpawner::ACucumberSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACucumberSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACucumberSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACucumberSpawner::MakeCucumber()
{
	auto Cucumber = GetWorld()->SpawnActor<ACucumber>(CucumberFactory, SpawnArrow->GetComponentTransform());
}
