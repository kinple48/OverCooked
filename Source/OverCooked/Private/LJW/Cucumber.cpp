// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Cucumber.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACucumber::ACucumber()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);

}

// Called when the game starts or when spawned
void ACucumber::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACucumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACucumber::Multicast_ChopCucumber_Implementation()
{
	if (ChoppedMesh)
	{
		meshcomp->SetStaticMesh(ChoppedMesh);
		meshcomp->SetRelativeScale3D(FVector(1.f));
		//boxcomp->SetRelativeScale3D(FVector(5.f));
		ForceNetUpdate();
	}
}



void ACucumber::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACucumber, bCooked);

}
