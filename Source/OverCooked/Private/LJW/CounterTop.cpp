// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/CounterTop.h"
#include "Components/BoxComponent.h"

// Sets default values
ACounterTop::ACounterTop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);
	SnapPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	Tags.Add(FName("Snappable"));
}

// Called when the game starts or when spawned
void ACounterTop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACounterTop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

