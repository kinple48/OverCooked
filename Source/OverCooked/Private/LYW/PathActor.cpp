// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/PathActor.h"
#include "Components/SplineComponent.h"
#include "LYW/HangerActor.h"

// Sets default values
APathActor::APathActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Root);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	Spline->bDrawDebug = true;
}

// Called when the game starts or when spawned
void APathActor::BeginPlay()
{
	Super::BeginPlay();

	if (ActorFactory)
	{
		Hanger = GetWorld()->SpawnActor<AHangerActor>(ActorFactory, Spline->GetComponentTransform());
	}
}

void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanMove) return;

	ElapsedTime += DeltaTime;

	float TimeInPeriod = FMath::Fmod(ElapsedTime, Period);

	float Alpha = TimeInPeriod / LoopDuration;
	if (Alpha > 1.0f)
	{
		// 두 번째 절반(끝→시작) 구간에선 1→0으로 뒤집음
		Alpha = 2.0f - Alpha;
	}

	float Distance = Alpha * Spline->GetSplineLength();

	// 6) 위치 및 회전 업데이트
	const FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	const FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	Hanger->SetActorLocation(Position);
	Hanger->SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
}

