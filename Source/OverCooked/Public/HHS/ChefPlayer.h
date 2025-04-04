// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChefPlayer.generated.h"

UCLASS()
class OVERCOOKED_API AChefPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChefPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC_Player;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Move;
	
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	float WalkSpeed = 400.0f;

	FVector Direction;

	void Move(const struct FInputActionValue& InputValue);
};
