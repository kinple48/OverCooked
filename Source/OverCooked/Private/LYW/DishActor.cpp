// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/DishActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LYW/SushiMeshData.h"
#include "LJW/Rice.h"
#include "LJW/Cucumber.h"
#include "LJW/SeaWeed.h"
#include "LJW/Fish.h"
#include "LYW/DishIngredientUI.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADishActor::ADishActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(50.0f));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(BoxComp);

	DishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DishMesh"));
	DishMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
	DishMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DishMesh->SetupAttachment(BoxComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/CookAsset/Mesh/plate20.plate20'"));

	if (tempMesh.Succeeded())
		DishMesh->SetStaticMesh(tempMesh.Object);

	IngredientComp = CreateDefaultSubobject<USceneComponent>(TEXT("IngredientComp"));
	IngredientComp->SetupAttachment(RootComponent);
	IngredientComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	RiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RiceMesh"));
	RiceMesh->SetupAttachment(IngredientComp);
	RiceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RiceMesh->SetCastShadow(false);

	SalmonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SalmonMesh"));
	SalmonMesh->SetupAttachment(IngredientComp);
	SalmonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SalmonMesh->SetCastShadow(false);

	SeaWeedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SeaWeedMesh"));
	SeaWeedMesh->SetupAttachment(IngredientComp);
	SeaWeedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SeaWeedMesh->SetCastShadow(false);

	CucumberMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CucumberMesh"));
	CucumberMesh->SetupAttachment(IngredientComp);
	CucumberMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CucumberMesh->SetCastShadow(false);

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodMesh->SetupAttachment(IngredientComp);
	FoodMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.3f));
	FoodMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.33f));
	FoodMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FoodMesh->SetCastShadow(false);

	IngreUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("IngredientUI"));
	IngreUI ->SetupAttachment(RootComponent);
	IngreUI->SetCastShadow(false);
}


// Called when the game starts or when spawned
void ADishActor::BeginPlay()
{
	Super::BeginPlay();
	RiceMesh->SetHiddenInGame(true);
	SeaWeedMesh->SetHiddenInGame(true);
	SalmonMesh->SetHiddenInGame(true);
	CucumberMesh->SetHiddenInGame(true);
	FoodMesh->SetHiddenInGame(true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADishActor::OnDishActorBeginOverlap);
	//IngreUI = Cast<UDishIngredientUI>(CreateWidget(GetWorld(), IngredientUIFactory));
}

// Called every frame
void ADishActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (IngreUI)
	{
		FVector CampLoc = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FVector Dir = CampLoc - IngreUI->GetComponentLocation();

		Dir.Z = 0.0f;
		IngreUI->SetWorldRotation(Dir.GetSafeNormal().ToOrientationRotator());
	}*/
}

//{ TEXT("SeaWeed"), TEXT("Rice"), TEXT("Cucumber"), TEXT("Salmon") };

void ADishActor::CheckIngredient()
{
	Key = "";
	for (const FString& Ingredient : IngredientArr)
	{
		if (IngredientsSet.Contains(Ingredient))
			Key += "1";
		else
			Key += "0";
	}

	if (Key == FString("1110"))
	{
		HideIngredients();
		// mesh change
		RollMesh->SetMesh_CucumberSushi(this);
	}
	else if (Key == FString("1101"))
	{
		HideIngredients();
		// mesh change
		RollMesh->SetMesh_SalmonSushi(this);
	}
	else if (Key == FString("1111"))
	{
		HideIngredients();
		RollMesh->SetMesh_MixedSushi(this);
		// mesh change
	}
	else
	{
		return;
	}
}

void ADishActor::AddSalmon()
{
	IngredientsSet.Add("Salmon");
	SalmonMesh->SetHiddenInGame(false);
	CheckIngredient();
	
	if (auto UI = Cast<UDishIngredientUI>(IngreUI->GetWidget()))
	{
		UI->FillImg("Salmon");
	}
	
}

void ADishActor::AddSeaWeed()
{
	IngredientsSet.Add("SeaWeed");
	SeaWeedMesh->SetHiddenInGame(false);
	CheckIngredient();
	if (auto UI = Cast<UDishIngredientUI>(IngreUI->GetWidget()))
	{
		UI->FillImg("SeaWeed");
	}
}

void ADishActor::AddRice()
{
	IngredientsSet.Add("Rice");
	RiceMesh->SetHiddenInGame(false);
	CheckIngredient();
	if (auto UI = Cast<UDishIngredientUI>(IngreUI->GetWidget()))
	{
		UI->FillImg("Rice");
	}
}

void ADishActor::AddCucumber()
{
	IngredientsSet.Add("Cucumber");
	CucumberMesh->SetHiddenInGame(false);
	CheckIngredient();
	if (auto UI = Cast<UDishIngredientUI>(IngreUI->GetWidget()))
	{
		UI->FillImg("Cucumber");
	}
}

void ADishActor::HideIngredients()
{
	RiceMesh->SetHiddenInGame(true);
	SeaWeedMesh->SetHiddenInGame(true);
	SalmonMesh->SetHiddenInGame(true);
	CucumberMesh->SetHiddenInGame(true);
	FoodMesh->SetHiddenInGame(false);
}

void ADishActor::OnDishActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto rice = Cast<ARice>(OtherActor))
	{
		if (rice->bCooked)
		{
			AddRice();
			rice->Destroy();
		}
	}
	else if (auto cucumber = Cast<ACucumber>(OtherActor))
	{
		if (cucumber->bCooked)
		{
			AddCucumber();
			cucumber->Destroy();
		}
	}
	else if (auto seaweed = Cast<ASeaWeed>(OtherActor))
	{
		AddSeaWeed();
		seaweed->Destroy();
	}

	else if (auto fish = Cast<AFish>(OtherActor))
	{
		if (fish->bCooked)
		{
			AddSalmon();
			fish->Destroy();
		}
	}
}
