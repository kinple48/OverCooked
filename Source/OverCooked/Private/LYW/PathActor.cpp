// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/PathActor.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"

// Sets default values
APathActor::APathActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Root);

	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	Spline->bDrawDebug = true;
}

// Called when the game starts or when spawned
void APathActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Spline Transform: %s"), *Spline->GetComponentTransform().ToString());
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle,
			this,
			&APathActor::SpawnHanger,
			SpawnInterval,  // 반복 주기
			true,           // 루프 여부
			StartTime       // 최초 지연
		);
	}
}

void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanMove || !Hanger) return;

	// 경과 시간 누적
	ElapsedTime += DeltaTime;

	// 전체 이동 시간 대비 진행률
	float Alpha = ElapsedTime / MoveDuration;

	// 끝점 도착 시 Destroy
	if (Alpha >= 1.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hanger reached the end. Destroying..."));
		Hanger->Destroy();
		Hanger = nullptr;
		bCanMove = false;
		return;
	}

	// 위치 및 회전 계산
	float Distance = Alpha * Spline->GetSplineLength();
	const FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	const FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	Hanger->SetActorLocation(Position);
	Hanger->SetActorRotation(Rotation);
}

void APathActor::SpawnHanger()
{
	if (Hanger)  // 기존 Hanger가 살아있으면 먼저 Destroy
	{
		Hanger->Destroy();
		Hanger = nullptr;
		bCanMove = false;
	}

	if (ActorFactory)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ElapsedTime = 0.0f;
		Hanger = GetWorld()->SpawnActor<ACharacter>(ActorFactory, Spline->GetComponentTransform(), Params);
		bCanMove = true;
		UE_LOG(LogTemp, Warning, TEXT("I Made Hanger"));
		if (Hanger)
		{
			Hanger->SetReplicates(true);  // 이거 중요!
			Hanger->SetReplicateMovement(true);  // 움직임도 동기화할 거면
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I don't have Stranger Factory"));
	}
}
