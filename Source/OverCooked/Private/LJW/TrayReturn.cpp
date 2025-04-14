// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/TrayReturn.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "LJW/DirtyDish.h"

// Sets default values
ATrayReturn::ATrayReturn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meschcomp"));
	meshcomp->SetupAttachment(boxcomp);

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(boxcomp);

	Tags.Add(FName("Snappable"));
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
	Super::Tick(DeltaTime);
	if (DishCount > 0)
	{
		curTime += DeltaTime;
		if (curTime >= maxTime)
		{
			GetWorld()->SpawnActor<ADirtyDish>(DishFactory, SpawnArrow->GetComponentTransform());
			DishCount -= 1;
			curTime = 0.f;
		}
	}
}

