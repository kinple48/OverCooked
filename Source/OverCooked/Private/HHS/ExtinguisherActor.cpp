// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/ExtinguisherActor.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AExtinguisherActor::AExtinguisherActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	//Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Tags.Add("Extinguisher");

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->bAutoActivate = false;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Steam_Lit"));
	if (ParticleAsset.Succeeded())
	{
		FireEffect->SetTemplate(ParticleAsset.Object);
	}
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
	if (FireEffect)
	{
		FireEffect->ActivateSystem();
	}
}

