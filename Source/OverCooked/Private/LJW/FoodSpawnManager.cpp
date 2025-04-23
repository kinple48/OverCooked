// Fill out your copyright notice in the Description page of Project Settings.

#include "LJW/FoodSpawnManager.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "LJW/Rice.h"
#include "LJW/SeaWeed.h"
#include "LJW/Fish.h"

// Sets default values
AFoodSpawnManager::AFoodSpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(RootComponent);
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

	if (!HasAuthority()) return;

	CurrentTime += DeltaTime;

	if (CurrentTime >= MakeTime)
	{
		if (foodtype)
		{
			if (state)
			{
				MakeRice();
				state = false;
				CurrentTime = 0.f;
			}

			else
			{
				MakeSeaWeed();
				state = true;
				CurrentTime = 0.f;
			}
		}
		else
		{
			MakeFish();
			CurrentTime = 0.f;
		}
	}
}

void AFoodSpawnManager::MakeRice()
{
	auto Rice = GetWorld()->SpawnActor<ARice>(RiceFactory, SpawnArrow->GetComponentTransform());
	Rice->SetReplicates(true);
	Rice->SetReplicateMovement(true);
}

void AFoodSpawnManager::MakeSeaWeed()
{
	auto SeaWeed = GetWorld()->SpawnActor<ASeaWeed>(SeaWeedFactory, SpawnArrow->GetComponentTransform());
	SeaWeed->SetReplicates(true);
	SeaWeed->SetReplicateMovement(true);
}

void AFoodSpawnManager::MakeFish()
{
	auto Fish = GetWorld()->SpawnActor<AFish>(FishFactory, SpawnArrow->GetComponentTransform());
	Fish->SetReplicates(true);
	Fish->SetReplicateMovement(true);
}
