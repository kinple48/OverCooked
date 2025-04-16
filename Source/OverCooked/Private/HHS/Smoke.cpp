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
	CollisionComp->InitSphereRadius(50.f);
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

	if ( SmokeMesh && SmokeMesh->GetMaterial(0))
	{
		DynamicSmoke = UMaterialInstanceDynamic::Create(SmokeMesh->GetMaterial(0), this);
		SmokeMesh->SetMaterial(0, DynamicSmoke);
	}
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASmoke::OnOverlapBegin);
}

// Called every frame
void ASmoke::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Purple, false, 0.1f);

	ElapsedTime += DeltaTime;

	TArray<AActor*> OverlapActor;
	CollisionComp->GetOverlappingActors(OverlapActor, ATestFire::StaticClass());

	for (AActor* Actor : OverlapActor)
	{
		ATestFire* Fire = Cast<ATestFire>(Actor);
		if (Fire)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Smoke] Overlapping with Fire: %s"), *Fire->GetName());

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

void ASmoke::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto fire = Cast<ATestFire>(OtherActor);
	if (fire)
	{
		fire->Destroy();
	}
}

