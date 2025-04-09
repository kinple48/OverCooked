// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/ServingStation.h"
#include "Components/BoxComponent.h"

// Sets default values
AServingStation::AServingStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);
}

// Called when the game starts or when spawned
void AServingStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AServingStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

