// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/CarSpawnManager.h"

#include "Components/ArrowComponent.h"
#include "HHS/Car.h"

// Sets default values
ACarSpawnManager::ACarSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACarSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
	SetReplicateMovement(true);
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle,
			this,
			&ACarSpawnManager::SpawnCar,
			SpawnInterval,  // 반복 주기
			true,           // 루프 여부
			StartTime       // 최초 지연
		);
	}
}

void ACarSpawnManager::SpawnCar()
{
	if (CarClass)
	{
		FActorSpawnParameters SpawnParams;
		CarActor = GetWorld()->SpawnActor<ACarActor>(CarClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
		if (CarActor)
		{
			CarActor->SetReplicates(true);  // 이거 중요!
			CarActor->SetReplicateMovement(true);  // 움직임도 동기화할 거면
		}
		//CarActor->DelayTime = SpawnInterval;
		//CarActor->canMove = true;
	}
}