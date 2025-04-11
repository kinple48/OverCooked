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
	arrowcomp->SetupAttachment(boxcomp);
	arrowcomp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));


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
}

// Called every frame
void AGasStove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTimerOn)
	{
		CurTime += DeltaTime;

		if (CurTime >= FireTime)
		{
			GetWorld()->SpawnActor<ATestFire>(FireFactory, arrowcomp->GetComponentTransform());
			bTimerOn = false;
			return;
		}
		else if (CurTime >= OverCookedTime)
		{
			SetWidgetTo(OverCookedUIClass);
		}
		else if (CurTime >= MaxTime)
		{
			SetWidgetTo(CookedUIClass);
		}
		else
		{
			SetWidgetTo(TimerUIClass);
		}
		
		if (TimeUI)
		{
			TimeUI->Time = FMath::Clamp(CurTime / MaxTime, 0.f, 1.f);
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
		bTimerOn = false;
		CurTime = 0.f;
	}
}

void AGasStove::SetWidgetTo(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (TimerWidget && TimerWidget->GetWidgetClass() != NewWidgetClass)
	{
		TimerWidget->SetWidgetClass(NewWidgetClass);
		TimerWidget->InitWidget();
		TimerWidget->SetVisibility(true);

		TimeUI = Cast<UTimerUI>(TimerWidget->GetWidget());
	}
}
