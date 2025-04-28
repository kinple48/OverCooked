// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/StrangerManager.h"
#include "LYW/PathActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStrangerManager::AStrangerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AStrangerManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> TempList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), TempList);
	UE_LOG(LogTemp, Warning, TEXT("I Find %d Path"), TempList.Num());


	for (AActor* Actor : TempList)
	{
		if (APathActor* PathActor = Cast<APathActor>(Actor))
		{
			PathList.Add(PathActor);
		}
	}
}

void AStrangerManager::Tick(float DeltaTime)
{
	if (HasAuthority())
	{
		PeriodCurrentTime += DeltaTime;
		if (bCanSpawn)
		{
			HandleSpawn(DeltaTime);
		}
		if (PeriodCurrentTime > PeriodInterval)
		{
			PeriodCurrentTime = 0.0f;
			bCanSpawn = true;
		}
		
	}
}

void AStrangerManager::HandleSpawn(float DeltaTime)
{
	SpawnCurrentTime += DeltaTime;
	if (SpawnCurrentTime > SpawnInterval)
	{
		if (makeCount < PathList.Num())
		{
			if (PathList.IsValidIndex(makeCount))
			{
				PathList[makeCount]->SpawnHanger();
				SpawnCurrentTime = 0.0f;
			}

			if (makeCount == PathList.Num() - 1)
			{
				bCanSpawn = false;
			}
			else
			{
				makeCount++;
			}
		}
	}
}
