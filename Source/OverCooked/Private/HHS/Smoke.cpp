// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Smoke.h"

// Sets default values
ASmoke::ASmoke()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SmokeMesh = CreateDefaultSubobject<UStaticMeshComponent>("SmokeMesh");
	RootComponent = SmokeMesh;
}

// Called when the game starts or when spawned
void ASmoke::BeginPlay()
{
	Super::BeginPlay();

	if ( SmokeMesh && SmokeMesh->GetMaterial(0))
	{
		DynamicSmoke = UMaterialInstanceDynamic::Create(SmokeMesh->GetMaterial(0), this);
		SmokeMesh->SetMaterial(0, DynamicSmoke);
	}
}

// Called every frame
void ASmoke::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	// 연기 위로 이동
	FVector Direction = GetActorForwardVector();
	FVector NewLocation = GetActorLocation() + Direction * SmokeSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	// 점점 투명해짐
	if ( ElapsedTime > FadeStartTime && DynamicSmoke )
	{
		float Alpha = 1.0f - ((ElapsedTime - FadeStartTime) / (LifeTime - FadeStartTime));
		DynamicSmoke->SetScalarParameterValue("OpacityAmount", FMath::Clamp(Alpha, 0.0f, 1.0f));
	}

	// 사라짐
	if (ElapsedTime > LifeTime )
	{
		Destroy();
	}
}

