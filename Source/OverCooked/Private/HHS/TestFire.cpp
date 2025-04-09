// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/TestFire.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATestFire::ATestFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	RootComponent = FireParticle;
	FireParticle->bAutoActivate = true;
	bIsExtinguished = false;
	
	FireParticle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FireParticle->SetCollisionResponseToAllChannels(ECR_Ignore);
	FireParticle->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(FireParticle);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called when the game starts or when spawned
void ATestFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestFire::Extinguish()
{
	if (!bIsExtinguished && FireParticle)
	{
		FireParticle->DeactivateSystem();
		bIsExtinguished = true;
		UE_LOG(LogTemp, Warning, TEXT("불 꺼짐"));
	}
}

