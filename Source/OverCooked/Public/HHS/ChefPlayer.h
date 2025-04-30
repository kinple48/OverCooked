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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* CircleUI;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hat")
	UStaticMeshComponent* HatMesh;

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

	UPROPERTY(Replicated, VisibleAnywhere, Category="Movement")
	bool bIsDashing = false;

	UPROPERTY(Replicated)
	bool bCanDash = true;
	

public:
	UFUNCTION()
	void Dash();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Dash();

	UFUNCTION()
	void StopDash();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StopDash();

	UFUNCTION()
	void ResetDash();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ResetDash();
	
	UPROPERTY(Replicated)
	AActor* HoldingActor = nullptr;
	
	// 잡을 범위
	UPROPERTY(EditAnywhere, Category="Grab")
	float GrabRadius = 50.0f;


	void GraborDrop();
	void DropObject();
	void GrabObject();
	
	void Chop();
	void Throw();
	void FireExtinguisher();
	
	void StopExtinguisher();
	void OnInteractPressed();
	void OnInteractReleased();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Extinguisher")
	bool bIsUsingExtinguisher = false;
	bool bIsThrowing = false;


	bool IsHoldingExtinguisher() const;
	bool IsHoldingActor() const;
	bool IsChoppingBoard() const;


	UPROPERTY(Replicated)
	bool bIsChopping = false;

	UPROPERTY(Replicated)
	int32 ChopCount = 0;

	UPROPERTY(Replicated)
	int32 MaxChopCount = 5;

	float ChopTimer = 0.f;
	bool bIsChopped = false;	
	
	UPROPERTY(Replicated)
	ACuttingBoard* NearBoard; // ACuttingBoard
	
	UPROPERTY(Replicated)
	ACuttingBoard* CuttingBoard;

	AFoodBox* HitFoodBox;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(Replicated)
	UAnimMontage* ChopMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chopping")
	bool bIsChopAnim;
	
	UPROPERTY(Replicated)
	bool bIsWashing = false;	
	int32 WashCount = 0;
	int32 MaxWashCount = 5;
	float WashTimer = 0.f;
	bool bIsWashed = false;

	UPROPERTY(Replicated)
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

	UPROPERTY(Replicated)
	AKnife* Knife;

	void holdKnife();
	void UnholdKnife();

	UPROPERTY(Replicated)
	bool bSink = false;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Chopping")
	bool bCutting = false;


	void Death();
	void Respawn();
	
	FTimerHandle RespawnTimer;
	FVector RespawnLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UFUNCTION(Server, Reliable)
	void ServerRPC_GrabObject();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_GrabObject(AActor* TargetActor, AFoodBox* SourceFoodBox);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Dropobject();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Dropobject(AActor* DroppedActor, FVector Location, FRotator Rotation, AActor* SnapTarget);
	
	UFUNCTION(Server, Reliable)
	void Server_FireExtinguisher();

	UFUNCTION(Server, Reliable)
	void Server_StopExtinguisher();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ActivateExtinguisher();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_DeactivateExtinguisher();

	UFUNCTION(Server, Reliable)
	void Server_SetLookDirection(const FVector& LookDirection);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetLookDirection(const FRotator& Rotation);

	UFUNCTION(Server, Reliable)
    void Server_Throw(const FVector& ThrowDirection);

    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_Throw(AActor* ThrownActor, const FVector& ThrowDirection);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Chop();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChopFinished();

	UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_PlayChopMontage();

    UFUNCTION(Server, Reliable)
    void ServerRPC_AttachKnifeFromBoard(ACuttingBoard* Board);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_AttachKnife(AKnife* KnifeToAttach);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetCuttingBoard(ACuttingBoard* Board);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetNearSink(ASink* DetectedSink);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Wash();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayWashMontage();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WashFinished();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere,Category = "Sound")
	class USoundCue* MySoundCue;

	UPROPERTY(EditAnywhere,Category = "Sound")
	class USoundBase* ChopSound;
};
