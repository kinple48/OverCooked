// Fill out your copyright notice in the Description page of Project Settings.

#include "OverCooked/Public/HHS/ChefPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/ExtinguisherActor.h"
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
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

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
	
	if ( bIsThrowing && IsHoldingActor() )
	{	
		FVector Offset = FVector(80.0f, 0.0f, 0.0f);
		FVector TargetLocation = GetActorLocation() + GetActorRotation().RotateVector(Offset);
		HoldingActor->SetActorLocation(TargetLocation);
		HoldingActor->SetActorRotation(FRotator::ZeroRotator);

		// 8방향
		if (!Direction.IsNearlyZero())
		{
			FVector Forward = FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal();
			SetActorRotation(Forward.Rotation());
		}
		return; // 이동하지 않음
	}
	if (!bIsUsingExtinguisher)
	{
		AddMovementInput(Direction);
	}
	else
	{
		if (!Direction.IsNearlyZero())
		{
			FVector Forward = FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal();
			SetActorRotation(Forward.Rotation()	);
		}
	}
	Direction = FVector::ZeroVector;

	if (bIsChopping && NearBoard )
	{
		float Distance = FVector::Dist(GetActorLocation(), NearBoard->GetActorLocation());
		float MaxChopDistance = 200.f;

		if (Distance > MaxChopDistance)
		{
			// 거리 벗어나면 다지기 중단
			bIsChopping = false;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("거리 멀음"));
			return;
		}
		ChopTimer += DeltaTime;
		float ChopDelay = 0.5f; // 0.5초에 한 번 카운트

		if (ChopTimer >= ChopDelay)
		{
			ChopTimer = 0.f;
			ChopCount++;

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("다지기 횟수: %d / %d"), ChopCount, MaxChopCount));

			if (ChopCount >= MaxChopCount)
			{
				bIsChopping = false;
				ChopCount = 0;
				
				if (HoldingActor)
				{
					HoldingActor->Tags.AddUnique(FName("Chopped"));
				}

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("다지기 끝 ^ㅁ^"));
			}
		}
	}
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
		playerInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &AChefPlayer::OnInteractPressed);
		playerInput->BindAction(IA_Interact, ETriggerEvent::Completed, this, &AChefPlayer::OnInteractReleased);

	}
}

void AChefPlayer::Move(const struct FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();
	Direction.X = value.X;
	Direction.Y	= value.Y;
	
	// 8방향 저장
	if (!Direction.IsNearlyZero())
	{
		LastInputDirection = Direction;
	}
}


void AChefPlayer::Dash()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Emerald,TEXT("Dash"),true);
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
	if ( !HoldingActor ) return;

	FVector StartPoint = GetActorLocation()-FVector(0.0f,0.0f,50.0f);
	FVector EndPoint = StartPoint + GetActorForwardVector() * 50;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, StartPoint, EndPoint, ECC_Visibility, params);
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Blue, false, 2.f);

	HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HoldingActor->SetActorEnableCollision(true);

	UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
	if (BoxComp)
	{
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetEnableGravity(false);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UStaticMeshComponent* MeshComp = HoldingActor->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BoxComp 기준으로 (0,0,0)
			MeshComp->SetVisibility(true); 
		}
	}
	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->Tags.Contains(FName("Snappable")))
	{
		// Snappable 태그가 있는 액터라면
		AActor* HitActor = hitInfo.GetActor();
		USceneComponent* SnapPoint = HitActor->FindComponentByClass<USceneComponent>();
		if (SnapPoint && SnapPoint->GetFName() == FName("SnapPoint"))
		{
			// SnapPoint로 스냅
			FVector SnapLocation = SnapPoint->GetComponentLocation();
			HoldingActor->SetActorLocation(SnapLocation);
			HoldingActor->SetActorRotation(FRotator::ZeroRotator); // 회전 초기화
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, 
				FString::Printf(TEXT("%s에 스냅"), *HitActor->GetName()));
		}
		else
		{
			HoldingActor->SetActorLocation(HitActor->GetActorLocation());
			HoldingActor->SetActorRotation(FRotator::ZeroRotator);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, 
				FString::Printf(TEXT("%s에 스냅 (SnapPoint 없음)"), *HitActor->GetName()));
		}
	}
	else
	{
		// Snappable가 없으면 물리 적용
		if (BoxComp)
		{
			BoxComp->SetSimulatePhysics(true);
			BoxComp->SetEnableGravity(true);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}

	HoldingActor = nullptr;
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("잡은 오브젝트: %s"), *HitActor->GetName()), true);
			
			UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
			if (BoxComp)
			{
				BoxComp->SetSimulatePhysics(false);
				BoxComp->SetEnableGravity(false);	
				BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				BoxComp->SetCollisionProfileName(TEXT("NoCollision"));
				
				HoldingActor->SetActorEnableCollision(false);
				HoldingActor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				
				HoldingActor->SetActorRelativeLocation(FVector(80.f, 0.f, 0.f));
				HoldingActor->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
				
				HoldingActor->SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f)); 
			}
		}
	}
}


void AChefPlayer::Chop()
{
	if (!bIsChopping && NearBoard )
	{
		// 이미 다진 재료인지 확인
		if (HoldingActor && HoldingActor->Tags.Contains(FName("Chopped")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("이미 다진 재료"));
			return;
		}
		bIsChopping = true;
		ChopTimer = 0.f;
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange,TEXT("다지기"),true);
	}
	else if (bIsChopping)
	{
		ChopCount++;

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,FString::Printf(TEXT("다지기 횟수: %d / %d"), ChopCount, MaxChopCount));

		if (ChopCount >= MaxChopCount)
		{
			bIsChopping = false;
			ChopCount = 0;

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("다지기 끝^^"));
		}
	}
}

void AChefPlayer::Throw()
{
	if (!HoldingActor) return;
	
	UBoxComponent* BoxComp = HoldingActor->FindComponentByClass<UBoxComponent>();
	if (BoxComp)
	{
		HoldingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingActor->SetActorEnableCollision(true);
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetEnableGravity(true); //추가
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComp->SetCollisionResponseToAllChannels(ECR_Block);
		BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		
		// 8방향
		FVector ThrowDirection = FVector(LastInputDirection.X, LastInputDirection.Y, 0.f).GetSafeNormal();
		if (ThrowDirection.IsNearlyZero())
		{
			ThrowDirection = GetActorForwardVector();
		}
		BoxComp->AddImpulse(ThrowDirection * 700.f, NAME_None, true);
	}
	HoldingActor = nullptr;
}

void AChefPlayer::FireExtinguisher()
{
	if (!HoldingActor) return;

	AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor);
	if (Extinguisher)
	{
		bIsUsingExtinguisher = true;
		Extinguisher->ActivateExtinguisher();

		// 8방향
		if (!LastInputDirection.IsNearlyZero())
		{
			FVector LookDir = FVector(LastInputDirection.X, LastInputDirection.Y, 0.f).GetSafeNormal();
			SetActorRotation(LookDir.Rotation());
		}
		
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("소화기 사용"),true);
	}
}

void AChefPlayer::StopExtinguisher()
{
	bIsUsingExtinguisher = false;
	if (HoldingActor)
	{
		AExtinguisherActor* Extinguisher = Cast<AExtinguisherActor>(HoldingActor);
		if (Extinguisher)
		{
			Extinguisher->DeactivateExtinguisher();
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("소화기 중지"),true);

		}
	}
}

void AChefPlayer::OnInteractPressed()
{
	// 도마 감지
	FVector StartPoint = GetActorLocation()-FVector(0.0f,0.0f,50.0f);
	FVector EndPoint = StartPoint + GetActorForwardVector() * 50;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, StartPoint, EndPoint, ECC_Visibility, params);
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Blue, false, 2.f);

	if (bHit && hitInfo.GetActor() && hitInfo.GetActor()->ActorHasTag("CuttingBoard"))
	{
		CuttingBoard = Cast<ACuttingBoard>(hitInfo.GetActor());
		if (CuttingBoard)
		{
			NearBoard = CuttingBoard;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("도마 감지됨!"));
		}
	}
	if (IsHoldingExtinguisher())
	{
		FireExtinguisher();
	}
	else if (IsHoldingActor())
	{
		bIsThrowing = true;
	}
	else
	{
		Chop();
	}
}

void AChefPlayer::OnInteractReleased()
{
	if (IsHoldingExtinguisher())
	{
		StopExtinguisher();
	}
	else if (bIsThrowing && IsHoldingActor()) 
	{
		Throw();
		bIsThrowing = false;
	}
}


void AChefPlayer::ChopOrThrowOrExtinguish()
{
	if (IsHoldingExtinguisher())	// 1. 소화기 들고 있을 경우 -> 소화기 작동 ( 8방향 Tick )
	{
		FireExtinguisher();
	}
	else if (IsHoldingActor())	// 2. actor 들고 있을 경우 -> 던지기 ( 8방향 )
	{
		Throw();
	}
	else if (NearBoard)		// 3. 아무것도 안 들고 있음 -> 다지기 (애니메이션, 범위확인) / 도마 앞에서만 ( 도마 근처 판단 로직 )
	{
		Chop();
	}
}

bool AChefPlayer::IsHoldingExtinguisher() const
{
	return HoldingActor && HoldingActor->ActorHasTag("Extinguisher");
}

bool AChefPlayer::IsHoldingActor() const
{
	return HoldingActor != nullptr;
}

bool AChefPlayer::IsChoppingBoard() const
{
	return true;
}

void AChefPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,FString::Printf(TEXT("Overlap 감지: %s"), *OtherActor->GetName()));
	
	CuttingBoard = Cast<ACuttingBoard>(OtherActor);
	if (CuttingBoard && OtherActor->ActorHasTag("CuttingBoard"))
	{
		NearBoard = CuttingBoard;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("도마 근처 진입!"));
	}
}