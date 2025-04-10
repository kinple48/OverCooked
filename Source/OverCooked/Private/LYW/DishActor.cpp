// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/DishActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADishActor::ADishActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	DishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DishMesh"));
	DishMesh -> SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/CookAsset/Mesh/plate.plate'"));
	
	DishMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (tempMesh.Succeeded())
	{
		DishMesh->SetStaticMesh(tempMesh.Object);
		DishMesh->SetRelativeScale3D(FVector(20.0f));
	}

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(50.0f));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetupAttachment(RootComponent);

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodMesh->SetupAttachment(RootComponent);
	FoodMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADishActor::BeginPlay()
{
	Super::BeginPlay();
	for (auto str : IngredientArr)
	{
		Ingredient.Add(str, false);
	}
}

// Called every frame
void ADishActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const bool* isHave = Ingredient.FindKey(TEXT("Salmon"));
}

