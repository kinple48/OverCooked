// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Fish.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LJW/ConveyorBelt.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFish::AFish()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AFish::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &AFish::OnFishBeginOverlap);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &AFish::OnFishEndOverlap);
}

// Called every frame
void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (move)
	{
		FVector p = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;
		SetActorLocation(p);
	}
}

void AFish::OnFishBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = true;
	}
}

void AFish::OnFishEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto conveyorbelt = Cast<AConveyorBelt>(OtherActor);
	if (conveyorbelt)
	{
		move = false;
	}
}

float AFish::EaseOutSine(float x)
{
	return FMath::Sin((x * PI) / 2);
}

void AFish::StartScaleDown()
{
	//if (!HasAuthority()) return;
	GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, this, &AFish::UpdateScale, 0.02f, true);
	ScaleTimeElapsed = 0.0f;
}

void AFish::UpdateScale()
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

void AFish::Multicast_ChopFish_Implementation()
{
	if (ChoppedMesh)
	{
		meshcomp->SetStaticMesh(ChoppedMesh);
		boxcomp->SetRelativeScale3D(FVector(0.2f));
		boxcomp->SetRelativeRotation(FRotator(30.0f, 180.0f, 0.0f));
		ForceNetUpdate();
	}
}

void AFish::ServerRPC_ChopFish_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Fish=%s"), *GetName());
	if (!bCooked /* && HasAuthority()*/)
	{
		if (ChoppedMesh)
		{
			meshcomp->SetStaticMesh(ChoppedMesh);
			boxcomp->SetRelativeScale3D(FVector(0.2f));
			boxcomp->SetRelativeRotation(FRotator(30.0f, 180.0f, 0.0f));
			ForceNetUpdate();
		}

		UE_LOG(LogTemp, Log, TEXT("Fish=%s"), *GetName());
		bCooked = true;
		Multicast_ChopFish();
		ForceNetUpdate();
	}
}

void AFish::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFish, move);
	DOREPLIFETIME(AFish, bCooked);

}
