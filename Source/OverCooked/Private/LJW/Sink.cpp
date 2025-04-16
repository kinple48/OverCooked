// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Sink.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "LYW/DishActor.h"

// Sets default values
ASink::ASink()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);
	
	arrowcomp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowcomp"));
	arrowcomp->SetupAttachment(boxcomp);

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);
	SnapPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	Tags.Add(FName("Snappable"));
}

// Called when the game starts or when spawned
void ASink::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentEndOverlap.AddDynamic(this,&ASink::OnSinkEndOverLap);
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ASink::OnSinkBeginOverLap);
}

// Called every frame
void ASink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

inline void ASink::OnSinkEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bSnap = true;
}

void ASink::OnSinkBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	dish = Cast<ADishActor>(OtherActor);
}

void ASink::MakeDish()
{
	auto Dish = GetWorld()->SpawnActor<ADishActor>(DishFactory, arrowcomp->GetComponentTransform());
}
