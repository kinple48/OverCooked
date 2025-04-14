// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/DirtyDish.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADirtyDish::ADirtyDish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meschcomp1"));
	meshcomp->SetupAttachment(boxcomp);
}

// Called when the game starts or when spawned
void ADirtyDish::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADirtyDish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

