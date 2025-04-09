// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/FoodWasteBin.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "LJW/Rice.h"
#include "LJW/SeaWeed.h"
#include "LJW/Fish.h"

// Sets default values
AFoodWasteBin::AFoodWasteBin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);
}

// Called when the game starts or when spawned
void AFoodWasteBin::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &AFoodWasteBin::OnFoodWasteBinOverlap);
}

// Called every frame
void AFoodWasteBin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodWasteBin::OnFoodWasteBinOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Rice = Cast<ARice>(OtherActor);
	auto SeaWeed = Cast<ASeaWeed>(OtherActor);
	auto Fish = Cast<AFish>(OtherActor);

	if (Rice)
	{
		Rice->StartScaleDown();
	}

	if (SeaWeed)
	{
		SeaWeed->StartScaleDown();
	}
	
	if (Fish)
	{
		Fish->StartScaleDown();
	}
}
