// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/GasStove.h"
#include "Components/BoxComponent.h"
#include "LJW/Rice.h"
#include "Components/WidgetComponent.h"
#include "LJW/TimerUI.h"

AGasStove::AGasStove()
{
	PrimaryActorTick.bCanEverTick = true;
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(boxcomp);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	meshcomp->SetupAttachment(boxcomp);

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
			TimerWidget->SetWidget(CookedUI);
		}

		if (CurTime >= OverCookedTime)
		{
			TimerWidget->SetWidget(OverCookedUI);
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
	/*auto Rice = Cast<ARice>(OtherActor);
	if (Rice)
	{
		TimerWidget->SetVisibility(false);
		bTimerOn = false;
		CurTime = 0.f;
	}*/
}
