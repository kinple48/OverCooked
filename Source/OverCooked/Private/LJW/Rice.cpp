// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Rice.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LJW/ConveyorBelt.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

ARice::ARice()
{
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);
}

void ARice::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ARice::OnRiceBeginOverlap);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &ARice::OnRiceEndOverlap);
}

void ARice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (move)
	{
		FVector p = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;
		SetActorLocation(p);
	}
}

void ARice::OnRiceBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = true;
	}
}

void ARice::OnRiceEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = false;
	}
}

float ARice::EaseOutSine(float x)
{
	return FMath::Sin((x * PI) / 2);
}

void ARice::StartScaleDown()
{
	GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, this, &ARice::UpdateScale, 0.02f, true);
	ScaleTimeElapsed = 0.0f;
}

void ARice::UpdateScale()
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
	float RotationSpeed = 100.0f;
	CurrentRotation.Yaw += RotationSpeed * 0.02f;
	SetActorRotation(CurrentRotation);
	
}



