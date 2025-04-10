// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/ExtinguisherActor.h"

#include "HHS/TestFire.h"
#include "Particles/ParticleSystemComponent.h"

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

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->bAutoActivate = false;
	FireEffect->SetWorldScale3D(FVector(0.05f));
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

	if (FireEffect)
	{
		FireEffect->DeactivateSystem();  
		FireEffect->bAutoActivate = false;   
	}
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

	if (FireEffect)
	{
		FireEffect->DeactivateSystem();
		FireEffect->ResetParticles(); 
	}
}

void AExtinguisherActor::SprayOnce()
{
	if (FireEffect)
	{
		FireEffect->ActivateSystem(true); 
	}
	
	FVector Start = FireEffect->GetComponentLocation();
	FVector End = Start + FireEffect->GetForwardVector() * 300.f;
	FVector Middle = (Start + End) * 0.5f;
	DrawDebugSphere(GetWorld(), Middle, SphereRadius, 16, FColor::Cyan, false, 1.f);

	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults,Start,End,FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(SphereRadius),Params);
	
	for (auto& Hit : HitResults)
	{
		ATestFire* Fire = Cast<ATestFire>(Hit.GetActor());
		if (Fire && !Fire->bIsExtinguished)
		{
			FString Message = FString::Printf(TEXT("잡은 오브젝트: %s"),  *Hit.GetActor()->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, Message, true);

			Fire->Extinguish();
		}
	}
}

