// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/TestRice.h"

#include "Components/BoxComponent.h"
#include "LJW/ConveyorBelt.h"

ATestRice::ATestRice()
{
	PrimaryActorTick.bCanEverTick = true;
	
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	SetRootComponent(meshcomp);
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	boxcomp->SetupAttachment(meshcomp);
	boxcomp->SetBoxExtent(FVector(25.f, 25.f, 20.f)); 
}

void ATestRice::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ATestRice::OnRiceBeginOverlap);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &ATestRice::OnRiceEndOverlap);
}

void ATestRice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (move)
	{
		FVector p = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;
		SetActorLocation(p);
	}
}

void ATestRice::OnRiceBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = true;
	}
}

void ATestRice::OnRiceEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = false;
	}
}

float ATestRice::EaseOutSine(float x)
{
	return FMath::Sin((x * PI) / 2);
}

void ATestRice::StartScaleDown()
{
	GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, this, &ATestRice::UpdateScale, 0.02f, true);
	ScaleTimeElapsed = 0.0f;
}

void ATestRice::UpdateScale()
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



