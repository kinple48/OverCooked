// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/CarSpawnManager.h"

#include "Components/ArrowComponent.h"
#include "HHS/Car.h"

// Sets default values
ACarSpawnManager::ACarSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACarSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACarSpawnManager::SpawnCar, SpawnInterval, true);

	
}

// Called every frame
void ACarSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACarSpawnManager::SpawnCar()
{
	if (CarClass)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ACarActor>(CarClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}

