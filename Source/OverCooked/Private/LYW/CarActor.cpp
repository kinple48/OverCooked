// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/CarActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACarActor::ACarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(200.0f, 100.0f, 80.0f));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACarActor::HitPlayer);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CarBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
	CarBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarBodyMesh->SetupAttachment(RootComponent);
	CarBodyMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CarBodyMesh->SetRelativeLocation(FVector(-30.0f, 0.0f, -100.0f));

	CarWheelMesh_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_1"));
	CarWheelMesh_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarWheelMesh_1->SetupAttachment(CarBodyMesh);
	CarWheelMesh_1->SetRelativeLocation(FVector( 0.0f, -115.0f, 30.0f));

	CarWheelMesh_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_2"));
	CarWheelMesh_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarWheelMesh_2->SetupAttachment(CarBodyMesh);
	CarWheelMesh_2->SetRelativeLocation(FVector(0.0f, 115.0f, 30.0f));
}

// Called when the game starts or when spawned
void ACarActor::BeginPlay()
{
	Super::BeginPlay();
	currentTime = 0.0f;
	startPos = GetActorLocation();
}

// Called every frame
void ACarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canMove)
	{
		currentTime += DeltaTime;
		// p0 + vt

		if (currentTime < MovingTime)
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * MovingSpeed);
		}

		if (currentTime > DelayTime)
		{
			currentTime = 0.0f;
			SetActorLocation(startPos);
		}
	}
}

void ACarActor::HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

