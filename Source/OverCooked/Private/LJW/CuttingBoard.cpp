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

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);
	SnapPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	Tags.Add(FName("Snappable"));
}

// Called when the game starts or when spawned
void ACuttingBoard::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &ACuttingBoard::OnCuttingBoardEndOverLap);
}

// Called every frame
void ACuttingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACuttingBoard::OnCuttingBoardEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSnap = true;
}

