// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/FoodChecker.h"
#include "../../Public/LYW/DishActor.h"


// Sets default values
AFoodChecker::AFoodChecker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFoodChecker::BeginOverlap);
}

// Called when the game starts or when spawned
void AFoodChecker::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AFoodChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodChecker::PrintIngredient(ADishActor* dish)
{
	for (const FString& ing_str : IngredientArr)
	{
		bool* isHave = dish->Ingredient.Find(ing_str);

		FString ingredientName = ing_str;
		FString haveText = (isHave && *isHave) ? TEXT("true") : TEXT("false");

		UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *ingredientName, *haveText);
	}
}



void AFoodChecker::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DishActor = Cast<ADishActor>(OtherActor);
	if (DishActor)
	{
		PrintIngredient(DishActor);
	}
}