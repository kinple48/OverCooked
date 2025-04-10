// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/FoodChecker.h"
#include "../../Public/LYW/DishActor.h"
#include "Components/BoxComponent.h"


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

	for (int32 i = 0; i < 2; i++)
	{
		MakeRandomOrder();
	}
	currentTime = 0.0f;
}

// Called every frame
void AFoodChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;

	if (currentTime > newOrderTime && CurrentOrder.Num() < 5)
	{
		MakeRandomOrder();
	}
}

void AFoodChecker::PrintIngredient(ADishActor* dish)
{
	FString Key;
	Key = dish->Key;
	

	for (int32 i = 0; i < CurrentOrder.Num(); i++)
	{
		if (Key == CurrentOrder[i])
		{
			CurrentOrder.RemoveAt(i);
			MakeRandomOrder();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Key);
			break;
		}
	}

	// 잘못 제출 -> score 깎기???
}

void AFoodChecker::MakeRandomOrder()
{
	int32 menu = FMath::RandRange(0, 3);
	CurrentOrder.Add(OrderInfo[menu]);
	currentTime = 0.0f;
}

void AFoodChecker::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DishActor = Cast<ADishActor>(OtherActor);
	if (DishActor)
	{
		PrintIngredient(DishActor);
	}
}