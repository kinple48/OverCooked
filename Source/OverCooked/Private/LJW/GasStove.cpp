// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/GasStove.h"
#include "Components/BoxComponent.h"
#include "LJW/Rice.h"
#include "Components/WidgetComponent.h"
#include "LJW/TimerUI.h"
#include "Components/ArrowComponent.h"
#include "HHS/TestFire.h"

AGasStove::AGasStove()
{
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);

	arrowcomp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowcomp"));
	arrowcomp->SetupAttachment(arrowcomp);
	arrowcomp->SetRelativeRotation(FRotator(0.f,90.f,0.f));


	TimerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimerWidget"));
	TimerWidget->SetupAttachment(boxcomp);
	TimerWidget->SetCastShadow(false);
	TimerWidget->SetVisibility(false);
}

void AGasStove::BeginPlay()
{
	Super::BeginPlay();
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &AGasStove::OnGasStoveBeginOverlap);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &AGasStove::OnGasStoveEndOverlap);
	TimeUI = Cast<UTimerUI>(TimerWidget->GetWidget());
	CookedUI = CreateWidget<UTimerUI>(GetWorld(), CookedWidget);
	OverCookedUI = CreateWidget<UTimerUI>(GetWorld(), OverCookedWidget);
}

// Called every frame
void AGasStove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTimerOn)
	{
		CurTime += DeltaTime;
		float percent = CurTime / MaxTime;
		
		if(TimeUI)
		{
			TimeUI->Time = percent;
		}

		if (CurTime >= MaxTime)
		{
			if (CookedUI)
			{
				TimerWidget->SetWidget(CookedUI);
			}
		}

		if (CurTime >= OverCookedTime)
		{
			if (OverCookedUI)
			{
				TimerWidget->SetWidget(OverCookedUI);
			}
		}

		if (CurTime >= FireTime)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("fire453453535353"), true);
			GetWorld()->SpawnActor<ATestFire>(FireFactory, arrowcomp->GetComponentTransform());
			bTimerOn = false;
		}
	}
}

void AGasStove::OnGasStoveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Rice = Cast<ARice>(OtherActor);
	if (Rice)
	{
		TimerWidget->SetVisibility(true);
		bTimerOn = true;
	}
}

void AGasStove::OnGasStoveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Rice = Cast<ARice>(OtherActor);
	if (Rice)
	{
		TimerWidget->SetVisibility(false);
		TimerWidget->SetWidget(TimeUI);
		bTimerOn = false;
		CurTime = 0.f;
	}
}
