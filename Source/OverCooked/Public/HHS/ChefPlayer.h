// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LJW/CounterTop.h"
#include "LJW/CuttingBoard.h"
#include "LJW/FoodBox.h"
#include "LJW/Sink.h"
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
	int32 ChopCount = 0;
	int32 MaxChopCount = 5;
	float ChopTimer = 0.f;
	bool bIsChopped = false;	
	
	UPROPERTY()
	ACuttingBoard* NearBoard; // ACuttingBoard
	ACuttingBoard* CuttingBoard;
	AFoodBox* HitFoodBox;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ChopMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chopping")
	bool bIsChopAnim;
	
	UFUNCTION()
	void OnChopCountNotify();


	bool bIsWashing = false;
	int32 WashCount = 0;
	int32 MaxWashCount = 5;
	float WashTimer = 0.f;
	bool bIsWashed = false;

	ASink* NearSink;
	ASink* Sink;
	//AActor* WashingDish = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WashMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chopping")
	bool bIsWashAnim = false;

	void Wash();
	void OnWashCountNotify();

	class ACounterTop* CounterTop;
	class APot* Pot;
	class AFoodBox* FoodBox;
	class ADirtyDish* dirtydish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chop")
	AActor* Knife;

	void holdKnife();
	void UnholdKnife();

	bool bSink = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chopping")
	bool bCutting = false;


	
};

