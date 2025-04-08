// Fill out your copyright notice in the Description page of Project Settings.

#include "OverCooked/Public/HHS/ChefPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChefPlayer::AChefPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/cat/SKM_BlackCat.SKM_BlackCat'"));
	if (MeshTemp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTemp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-88.0f),FRotator(0.0f,0.0f,0.0f));
		GetMesh()->SetRelativeScale3D(FVector(2.5f));
	}
}

// Called when the game starts or when spawned
void AChefPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}

		ACameraActor* Camera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
		if (Camera)
		{
			pc->SetViewTargetWithBlend(Camera, 0.5f, VTBlend_Linear);
		}

	}
}

// Called every frame
void AChefPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void AChefPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);		
	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AChefPlayer::Move);
		playerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &AChefPlayer::Dash);
		playerInput->BindAction(IA_GraborDrop, ETriggerEvent::Started, this, &AChefPlayer::GraborDrop);
		playerInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &AChefPlayer::ChopOrThrowOrExtinguish);
	}
}

void AChefPlayer::Move(const struct FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();
	Direction.X = value.X;
	Direction.Y	= value.Y;
}


void AChefPlayer::Dash()
{
	UE_LOG(LogTemp, Warning, TEXT("Dash@@$@$@!"));
		
	if (!bCanDash || bIsDashing)
		return;

	bIsDashing = true;
	bCanDash = false;

	FVector DashDirection = GetActorForwardVector();
	LaunchCharacter(DashDirection * 2500.0f, true, true);
	
	// 대시 종료
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AChefPlayer::StopDash, DashDuration, false);
	// 쿨타임 
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AChefPlayer::ResetDash, DashCooldown, false);
}

void AChefPlayer::StopDash()
{
	bIsDashing = false;
	GetCharacterMovement()->StopMovementImmediately();
}

void AChefPlayer::ResetDash()
{
	bCanDash = true;
}

void AChefPlayer::GraborDrop()
{
	if (HoldingActor)
	{
		DropObject();
	}
	else
	{
		GrabObject();
	}
}

void AChefPlayer::DropObject()
{
	if (HoldingActor)
	{
		HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingActor->SetActorEnableCollision(true);
		HoldingActor = nullptr;
	}
}

void AChefPlayer::GrabObject()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 200.f;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
	DrawDebugSphere(GetWorld(), End, GrabRadius, 12, FColor::Green, false, 1.5f);
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if ( GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(GrabRadius),Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			HoldingActor = HitActor;
			HoldingActor->SetActorEnableCollision(false);
		
			HoldingActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		
			FVector Offset = FVector(80.0f, 0.0f, 0.0f); 
			HoldingActor->SetActorRelativeLocation(Offset);
			HoldingActor->SetActorRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

void AChefPlayer::Chop()
{
	UE_LOG(LogTemp, Warning, TEXT("다지기"));
}

void AChefPlayer::Throw()
{
	if (!HoldingActor) return;

	UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(HoldingActor->GetRootComponent());
	if (PrimitiveComp)
	{
		HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingActor->SetActorEnableCollision(true);
		
		FVector ThrowDirection = GetActorForwardVector();
		PrimitiveComp->SetSimulatePhysics(true);
		PrimitiveComp->AddImpulse(ThrowDirection * 1000.f, NAME_None, true);
	}
	HoldingActor = nullptr;
}

void AChefPlayer::UseFireExtinguisher()
{
	UE_LOG(LogTemp, Warning, TEXT("소화기"));
}

void AChefPlayer::ChopOrThrowOrExtinguish()
{
	// 1. 소화기 들고 있을 경우 -> 소화기 작동
	if (HoldingActor && HoldingActor->ActorHasTag("Extinguisher"))
	{
		UseFireExtinguisher();
		return;
	}

	// 2. actor 들고 있을 경우 -> 던지기
	if (HoldingActor)
	{
		Throw();
		return;
	}

	// 3. 아무것도 안 들고 있음 -> 다지기 (애니메이션, 범위확인)
	Chop(); 
}



