// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Car.h"

#include "Components/BoxComponent.h"
#include "HHS/ChefPlayer.h"

// Sets default values
ACar::ACar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);
	
	boxcomp->SetGenerateOverlapEvents(true);
	boxcomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxcomp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxcomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxcomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ACar::OnOverlapBegin);

	StartLocation = GetActorLocation(); 
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	if (FVector::Dist(StartLocation, NewLocation) > MaxDistance)
	{
		Destroy();
	}

}

void ACar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AChefPlayer* Player = Cast<AChefPlayer>(OtherActor);
	if (Player)
	{
		Player->Death();
	}
}
	
//ingredient