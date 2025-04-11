// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/ExtinguisherActor.h"
#include "HHS/TestFire.h"

// Sets default values
AExtinguisherActor::AExtinguisherActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComp);
	
	Tags.Add("Extinguisher");

	SmokeSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SmokeSpawnPoint"));
	SmokeSpawnPoint->SetupAttachment(Mesh);
	SmokeSpawnPoint->SetRelativeLocation(FVector(-30, 0, 0));
}


// Called when the game starts or when spawned
void AExtinguisherActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExtinguisherActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExtinguisherActor::ActivateExtinguisher()
{
	if (!GetWorldTimerManager().IsTimerActive(SprayTimerHandle))
	{
		SprayOnce();
		GetWorldTimerManager().SetTimer(SprayTimerHandle,this,&AExtinguisherActor::SprayOnce,0.3f,true);
	}
}

void AExtinguisherActor::DeactivateExtinguisher()
{
	GetWorldTimerManager().ClearTimer(SprayTimerHandle);

}

void AExtinguisherActor::SprayOnce()
{
	if (!SmokeClass) return;

	FVector SpawnLocation = SmokeSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = SmokeSpawnPoint->GetComponentRotation();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, SpawnRotation.ToString());


	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(SmokeClass, SpawnLocation, SpawnRotation, SpawnParams);
}