// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/FoodChecker.h"
#include "../../Public/LYW/DishActor.h"
#include "Components/BoxComponent.h"
#include "LYW/GameDataManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFoodChecker::AFoodChecker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComp->OnComponentEndOverlap
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFoodChecker::BeginOverlap);
}

// Called when the game starts or when spawned
void AFoodChecker::BeginPlay()
{
	Super::BeginPlay();
	DataManager = Cast<AGameDataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameDataManager::StaticClass()));

}

// Called every frame
void AFoodChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFoodChecker::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	given_dish = Cast<ADishActor>(OtherActor);
	if (given_dish)
	{
		FString given_order = given_dish->Key;
		DataManager->CheckOder(given_order);

		given_dish->Destroy();
		given_dish = nullptr;
	}
}