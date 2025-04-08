// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Dash;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_GraborDrop;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Interact;
	
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	float WalkSpeed = 400.0f;

	FVector Direction;

	void Move(const struct FInputActionValue& InputValue);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashStrength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown = 0.25f;

private:
	FTimerHandle DashTimerHandle;
	FTimerHandle CooldownTimerHandle;

	bool bIsDashing = false;
	bool bCanDash = true;

public:
	void Dash();
	void StopDash();
	void ResetDash();
	
	UPROPERTY()
	AActor* HoldingActor = nullptr;
	
	// 잡을 범위
	UPROPERTY(EditAnywhere, Category="Grab")
	float GrabRadius = 70.0f;

	void GraborDrop();
	void DropObject();
	void GrabObject();
	
	void Chop();
	void Throw();
	void UseFireExtinguisher();
	void ChopOrThrowOrExtinguish();



};
