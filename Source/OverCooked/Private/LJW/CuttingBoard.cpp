// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/CuttingBoard.h"
#include "Components/BoxComponent.h"

// Sets default values
ACuttingBoard::ACuttingBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp1"));
	meshcomp1->SetupAttachment(boxcomp);

	meshcomp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp2"));
	meshcomp2->SetupAttachment(boxcomp);
}

// Called when the game starts or when spawned
void ACuttingBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuttingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

