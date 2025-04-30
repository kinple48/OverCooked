// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/CarActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HHS/ChefPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/Sink.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACarActor::ACarActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(200.0f, 100.0f, 80.0f));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACarActor::HitPlayer);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CarBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
	CarBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarBodyMesh->SetupAttachment(RootComponent);
	CarBodyMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CarBodyMesh->SetRelativeLocation(FVector(-30.0f, 0.0f, -100.0f));

	CarWheelMesh_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_1"));
	CarWheelMesh_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarWheelMesh_1->SetupAttachment(CarBodyMesh);
	CarWheelMesh_1->SetRelativeLocation(FVector(0.0f, -115.0f, 30.0f));

	CarWheelMesh_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_2"));
	CarWheelMesh_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarWheelMesh_2->SetupAttachment(CarBodyMesh);
	CarWheelMesh_2->SetRelativeLocation(FVector(0.0f, 115.0f, 30.0f));
}

// Called when the game starts or when spawned
void ACarActor::BeginPlay()
{
	Super::BeginPlay();
	currentTime = 0.0f;
	startPos = GetActorLocation();
	SetReplicateMovement(true);
	SetReplicates(true);
	if (SpawnSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
		//UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
}

// Called every frame
void ACarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		currentTime += DeltaTime;
		// p0 + vt

		if (currentTime < MovingTime)
		{
			SetActorLocation(startPos + GetActorForwardVector() * MovingSpeed * currentTime);
		}

		else
		{
			//currentTime = 0.0f;
			//startPos = GetActorLocation();  // �Ǵ� �������� ����
			Destroy();
		}
	}
}

void ACarActor::MakeDish()
{
	ASink* sink = Cast<ASink>(UGameplayStatics::GetActorOfClass(GetWorld(), ASink::StaticClass()));
	if (sink)
	{
		sink->TimerMakeDish();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I can't Find sink"));
	}
}

void ACarActor::HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AChefPlayer* HitPlayer = Cast<AChefPlayer>(OtherActor);
		if (HitPlayer)
		{
			HitPlayer->Death();
			if (HitPlayer->HoldingActor)
			{
				
				if (HitPlayer->HoldingActor->ActorHasTag("dish"))
				{
					MakeDish();
				}
				HitPlayer->HoldingActor->Destroy();
			}
		}

		if (OtherActor->ActorHasTag("ingredient"))
		{
			UE_LOG(LogTemp, Warning, TEXT("I hit Indgredient"));
			OtherActor->Destroy();
		}

		if (OtherActor->ActorHasTag("dish"))
		{
			UE_LOG(LogTemp, Warning, TEXT("I hit dish"));
			OtherActor->Destroy();
			MakeDish();
			
		}
	}
}

void ACarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACarActor, currentTime);
}
