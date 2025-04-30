// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/CarSpawnManager.h"

#include "Components/ArrowComponent.h"
#include "HHS/Car.h"
#include "Kismet/GameplayStatics.h"

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
			SpawnInterval,  // �ݺ� �ֱ�
			true,           // ���� ����
			StartTime       // ���� ����
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
			CarActor->SetReplicates(true);  // �̰� �߿�!
			CarActor->SetReplicateMovement(true);  // �����ӵ� ����ȭ�� �Ÿ�
		}
		//if (CarSpawnSound)  
        //{
        //	UGameplayStatics::PlaySoundAtLocation(this, CarSpawnSound, GetActorLocation());
        //}
		//CarActor->DelayTime = SpawnInterval;
		//CarActor->canMove = true;
	}
}	