// Fill out your copyright notice in the Description page of Project Settings.


#include "LYW/HangerActor.h"

// Sets default values
AHangerActor::AHangerActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHangerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHangerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHangerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

