// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/SeaWeed.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LJW/ConveyorBelt.h"

ASeaWeed::ASeaWeed()
{
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);

	bReplicates = true;
}

void ASeaWeed::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ASeaWeed::OnSeaWeedBeginOverlap);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &ASeaWeed::OnSeaWeedEndOverlap);
}

void ASeaWeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (move)
	{
		FVector p = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;
		SetActorLocation(p);
	}
}

void ASeaWeed::OnSeaWeedBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = true;
	}
}


void ASeaWeed::OnSeaWeedEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = false;
	}
}

float ASeaWeed::EaseOutSine(float x)
{
	return FMath::Sin((x * PI) / 2);
}

void ASeaWeed::StartScaleDown()
{
	GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, this, &ASeaWeed::UpdateScale, 0.02f, true);
	ScaleTimeElapsed = 0.0f;
}

void ASeaWeed::UpdateScale()
{
	const float Duration = 1.0f;
	ScaleTimeElapsed += 0.02f;

	if (ScaleTimeElapsed >= Duration)
	{
		GetWorld()->GetTimerManager().ClearTimer(ScaleTimerHandle);
		Destroy();
		return;
	}

	float Alpha = ScaleTimeElapsed / Duration;
	float ScaleValue = EaseOutSine(1.0f - Alpha);

	SetActorScale3D(FVector(ScaleValue));

	FRotator CurrentRotation = GetActorRotation();
	float RotationSpeed = 180.0f;
	CurrentRotation.Yaw += RotationSpeed * 0.02f;
	SetActorRotation(CurrentRotation);
}

