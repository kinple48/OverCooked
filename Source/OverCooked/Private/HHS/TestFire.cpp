// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/TestFire.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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
	AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), firesound, 1.0f, 1.0f, 0.0f, nullptr, true);
	//DrawDebugBox(GetWorld(), GetActorLocation(), FVector(30, 30, 30), FColor::Red, false, 0.1f);

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
	}
}

