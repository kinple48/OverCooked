// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/TestFire.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATestFire::ATestFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetBoxExtent(FVector(30, 30, 30));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	FireParticle->SetupAttachment(CollisionBox);
	FireParticle->bAutoActivate = true;

	bIsExtinguished = false;

	FireParticle->bAutoActivate = true;
	bIsExtinguished = false;

}

// Called when the game starts or when spawned
void ATestFire::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(30, 30, 30), FColor::Red, false, 0.1f);

	
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
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Silver,TEXT("불 꺼짐"),true);

	}
}

