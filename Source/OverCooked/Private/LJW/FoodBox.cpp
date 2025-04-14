// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/FoodBox.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "LJW/Cucumber.h"

// Sets default values
AFoodBox::AFoodBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meschcomp1"));
	meshcomp1->SetupAttachment(boxcomp);

	meshcomp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp2"));
	meshcomp2->SetupAttachment(boxcomp);
	
	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(boxcomp);

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);
	SnapPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	Tags.Add(FName("Snappable"));
}

// Called when the game starts or when spawned
void AFoodBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

class ACucumber* AFoodBox::MakeCucumber()
{
	if (SnapActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("푸드박스 위에 액터있음"));
		return nullptr;
	}

	if (!CucumberFactory)
	{
		return nullptr;
	}

	FTransform SpawnTransform = SpawnArrow->GetComponentTransform();
	ACucumber* Cucumber = GetWorld()->SpawnActor<ACucumber>(CucumberFactory, SpawnTransform);
	if (Cucumber)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("오이 생성"));
	}
	return Cucumber;
}

void AFoodBox::SnappedActor(AActor* NewSnap)
{
	SnapActor = NewSnap;
}

void AFoodBox::UnSnappedActor()
{
	SnapActor = nullptr;
	bSnap = true;
}




