// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LJW/CuttingBoard.h"
#include "ChefPlayer.generated.h"

UENUM(BlueprintType)
enum class ECustomCollisionChannel : uint8
{
	ECC_None	UMETA(DisplayName = "None"),
	ECC_Grabbable = ECC_GameTraceChannel1 UMETA(DisplayName = "Grabbable")
};

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
	FTimerHandle ExtinguisherStopHandle;
	FVector HoldingActorLocation;
	FVector LastInputDirection;

	
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
	float GrabRadius = 100.0f;


	void GraborDrop();
	void DropObject();
	void GrabObject();
	
	void Chop();
	void Throw();
	void FireExtinguisher();
	void ChopOrThrowOrExtinguish();
	
	void StopExtinguisher();
	void OnInteractPressed();
	void OnInteractReleased();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Extinguisher")
	bool bIsUsingExtinguisher = false;
	bool bIsThrowing = false;


	bool IsHoldingExtinguisher() const;
	bool IsHoldingActor() const;
	bool IsChoppingBoard() const;


	bool bIsChopping = false;
	float ChoppingTime = 0.0f;

	UPROPERTY()
	ACuttingBoard* NearBoard;

	UPROPERTY(EditDefaultsOnly)
	float ChopDuration = 5.0f;
};
