// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Smoke.h"

#include "Components/SphereComponent.h"
#include "HHS/TestFire.h"

// Sets default values
ASmoke::ASmoke()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->InitSphereRadius(300.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	
	SmokeMesh = CreateDefaultSubobject<UStaticMeshComponent>("SmokeMesh");
	SmokeMesh->SetupAttachment(CollisionComp);
}

// Called when the game starts or when spawned
void ASmoke::BeginPlay()	
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[Smoke] Collision Enabled: %d"), CollisionComp->GetCollisionEnabled());
	UE_LOG(LogTemp, Warning, TEXT("[Smoke] Visibility Response: %d"), CollisionComp->GetCollisionResponseToChannel(ECC_Visibility));


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
	DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionComp->GetScaledSphereRadius(), 16, FColor::Green, false, 0.1f);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Purple, false, 0.1f);

	ElapsedTime += DeltaTime;

	TArray<AActor*> OverlapActor;
	CollisionComp->GetOverlappingActors(OverlapActor, ATestFire::StaticClass());

	for (AActor* Actor : OverlapActor)
	{
		ATestFire* Fire = Cast<ATestFire>(Actor);
		if (Fire)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("불이랑 겹침"));

			Fire->Extinguish();
		}
	}

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

