#include "LJW/GasStove.h"
#include "Components/BoxComponent.h"
#include "LJW/Rice.h"
#include "Components/WidgetComponent.h"
#include "LJW/TimerUI.h"
#include "Components/ArrowComponent.h"
#include "HHS/TestFire.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

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

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);
	SnapPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
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
			TimerWidget->SetRelativeScale3D(FVector(1.8f,1.8f,5.f));
			if (!playSoundWarning)
			{
				AudioComp2 = UGameplayStatics::SpawnSound2D(GetWorld(), WarningSound, 1.0f, 1.0f, 0.0f, nullptr, true);
				if (AudioComp2)
				{
					GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [this]()
					{
						if (AudioComp2) AudioComp2->Stop();
					}, 7.0f, false);
				}
				playSoundWarning = true;
			}
		}
		else if (CurTime >= MaxTime)
		{
			if (CookedUIClass)
			{
				SetWidgetTo(CookedUIClass);
				TimerWidget->SetRelativeScale3D(FVector(1.8f,1.8f,5.f));
			}
			if (Rice)
			{
				Rice->bCooked = true;
			}
		}
		else
		{
			SetWidgetTo(TimerUIClass);
			if (!playSoundBoiling)
			{
				AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), BoilingSound, 1.0f, 1.0f, 0.0f, nullptr, true);
				if (AudioComp)
				{
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
					{
						if (AudioComp) AudioComp->Stop();
					}, 19.0f, false);
				}
				playSoundBoiling = true;
			}
		}
		
		if (TimeUI)
		{
			TimeUI->Time = FMath::Clamp(CurTime / MaxTime, 0.f, 1.f);
		}
	}
}

void AGasStove::OnGasStoveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Rice = Cast<ARice>(OtherActor);
	if (Rice)
	{
		FVector SnapLocation = SnapPoint->GetComponentLocation();
		Rice->SetActorLocation(SnapLocation);
		
		TimerWidget->SetVisibility(true);
		bTimerOn = true;
	}
}

void AGasStove::OnGasStoveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Rice = Cast<ARice>(OtherActor);
	if (Rice)
	{
		TimerWidget->SetVisibility(false);
		TimerWidget->SetRelativeScale3D(FVector(1.f));
		bTimerOn = false;
		CurTime = 0.f;
		playSoundWarning = false;
		playSoundBoiling = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		if (AudioComp)AudioComp->Stop();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle2);
		if (AudioComp2)AudioComp2->Stop();
		//Rice = nullptr;
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
